#include "SimulatedAnnealing.h"

using namespace param;

random_device SimulatedAnnealing::rd;
mt19937 SimulatedAnnealing::mt(SimulatedAnnealing::rd());
uniform_int_distribution<int> SimulatedAnnealing::random_parameter(0, 3);
uniform_real_distribution<double> SimulatedAnnealing::random_0to1(0.0, 1.0);

void SimulatedAnnealing::setup(HandCursor* hc, unordered_map<long long int, SubWindow>* sub_windows) {
	this->hc = hc;
	this->sub_windows = sub_windows;
}

void SimulatedAnnealing::operator() (const unordered_map<long long int, ofRectangle>& start_state, unordered_map<long long int, ofRectangle>& best_state) {
	this->current_state = start_state;

	this->current_cost = this->next_cost = this->best_cost = 0.0;

	//this->ofs.open("cost_data" + to_string(this->file_index) + ".txt");
	for (int i = 0; i < this->MAX_ITERATION; ++i) {
		//this->ofs << this->current_cost << endl;

		if (!this->set_next_state()) { // パラメータの修正によって制約外の解になったらパラメータの修正を行わない
			continue;
		}

		this->calculate_cost();

		if (this->next_cost > this->current_cost) {
			//cout << exp(-1 * this->MAX_ITERATION * log(this->next_cost - this->current_cost) / 100000000 * i) << endl;
			if (random_0to1(this->mt) < exp(-1 * this->MAX_ITERATION * log(this->next_cost - this->current_cost) / 10000000 * i)) {
				this->current_cost = this->next_cost;
				this->current_state = this->next_state;
			}
		}
		else {
			this->current_cost = this->next_cost;
			this->current_state = this->next_state;

			if (this->current_cost < this->best_cost) {
				this->best_cost = this->current_cost;
				best_state = this->current_state;
			}
		}
	}
	//this->ofs.close();
	//++this->file_index;
}

bool SimulatedAnnealing::set_next_state() {
	this->next_state = this->current_state;

	uniform_int_distribution<int> random_window_num(0, this->sub_windows->size());
	auto i{ begin(*this->sub_windows) };
	advance(i, random_window_num(this->mt));

	this->modify_window_num = i->first; // 修正するウィンドウをランダムに決める

	this->modify_param = this->random_parameter(this->mt); // 修正するパラメータをランダムに決める
	switch (this->modify_param) {
	case 0:
	{
		uniform_int_distribution<int> random_x(0, DISPLAY_W - this->next_state[this->modify_window_num].getWidth());
		this->next_state[this->modify_window_num].setX(random_x(this->mt));
	}
	break;
	case 1:
	{
		uniform_int_distribution<int> random_y(0, DISPLAY_H - this->next_state[this->modify_window_num].getHeight());
		this->next_state[this->modify_window_num].setY(random_y(this->mt));
	}
	break;
	case 2:
	{
		uniform_int_distribution<int> random_w(100, 600);
		this->next_state[this->modify_window_num].setWidth(random_w(this->mt));
	}
	break;
	case 3:
	{
		uniform_int_distribution<int> random_h(100, 600);
		this->next_state[this->modify_window_num].setHeight(random_h(this->mt));
	}
	break;
	}

	for (const auto& s : this->next_state) {
		/* パラメータの修正によって制約外の解になったら */
		if (s.second.getLeft() < 0.005*DISPLAY_W || s.second.getRight() > 0.995*DISPLAY_W || s.second.getTop() < 0.005*DISPLAY_H || s.second.getBottom() > 0.995*DISPLAY_H || s.second.width > MAX_SUB_WINDOW_W || s.second.height > MAX_SUB_WINDOW_H) {
			return false;
		}
	}

	return true;
}

void SimulatedAnnealing::calculate_cost() {
	this->next_cost = 0.0;

	this->overlap_cost = 0.0;
	this->distance_cost = 0.0;

	for (const auto& s : this->next_state) {
		for (const auto& ud : this->hc->user_data) {
			//this->overlap_cost += s.second.getIntersection(ofRectangle(ofClamp(this->hc->user_data.at(id).transformed_cursor_point.x() - HALF_MAX_SUB_WINDOW_W, 0, DISPLAY_W), ofClamp(this->hc->user_data.at(id).transformed_cursor_point.y() - HALF_MAX_SUB_WINDOW_H, 0, DISPLAY_H), ofClamp(MAX_SUB_WINDOW_W, 0, DISPLAY_W), ofClamp(MAX_SUB_WINDOW_H, 0, DISPLAY_H))).getArea();
			if (s.second.intersects(ofRectangle(ofClamp(ud.second.transformed_cursor_point.x() - HALF_MAX_SUB_WINDOW_W, 0, DISPLAY_W), ofClamp(ud.second.transformed_cursor_point.y() - HALF_MAX_SUB_WINDOW_H, 0, DISPLAY_H), ofClamp(MAX_SUB_WINDOW_W, 0, DISPLAY_W), ofClamp(MAX_SUB_WINDOW_H, 0, DISPLAY_H)))) {
				// もし矩形とカーソルの周辺矩形が重複していたら、コストを最大にしてコスト計算を終了
				this->next_cost = 0.0;
				return;
			}
		}

		/* 重複面積の計算 */
		for (const auto& s2 : this->next_state) {
			if (s.second == s2.second) { // 自分との重複面積は除く
				continue;
			}
			this->overlap_cost += s.second.getIntersection(s2.second).getArea();
			if (this->overlap_cost > 0) { // もし重複していたら、コストを最大にしてコスト計算を終了
				this->next_cost = 0.0;
				return;
			}
		}

		this->next_cost -= s.second.getArea(); // 自分との重複面積分減らす

		/* 円形度 */
		this->shape_cost -= 4 * PI*s.second.getArea() / (s.second.getPerimeter()*s.second.getPerimeter());

		/* 矩形と顔との距離 */
		try {
			for (const auto& td : this->hc->user_data) {
				if (this->sub_windows->at(s.first).get_user_id() == td.first) {
					this->distance_cost += ofDist(s.second.getCenter().x, s.second.getCenter().y, td.second.transformed_face_point.x(), td.second.transformed_face_point.y());
					continue;
				}
			}
		}
		catch (std::out_of_range&) {}
	}

	this->area_cost = -min_element(begin(this->next_state), end(this->next_state), [](const pair<int, ofRectangle>& a, const pair<int, ofRectangle>& b) {return a.second.getArea() < b.second.getArea(); })->second.getArea();

	//this->next_cost += 150 * this->area_cost + 10000 * this->overlap_cost + 1000 * this->shape_cost + 700 * this->distance_cost;
	this->next_cost += 150 * this->area_cost + 10000 * this->overlap_cost + 700 * this->distance_cost;
}