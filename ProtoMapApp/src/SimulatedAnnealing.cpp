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

void SimulatedAnnealing::operator() (const unordered_map<long long int, ofRectangle>& start_state, unordered_map<long long int, ofRectangle>& best_state, double& best_cost) {
	this->current_state = start_state;
	this->past_cost = this->current_cost = this->next_cost = this->best_cost = 0.0;
	this->convergence_count = 0;

	//this->ofs.open("cost_data" + to_string(this->file_index) + ".txt");
	for (int i = 0; i < this->MAX_ITERATION; ++i) {
		//this->ofs << this->current_cost << endl;

		if (!this->set_next_state()) { // パラメータの修正によって制約外の解になったらパラメータの修正を行わない
			continue;
		}

		this->calculate_cost();

		/* 一定のイテレーション数で同じコストが続いたら、収束したとして、ループを抜ける*/
		this->convergence_count = (this->current_cost == this->past_cost && this->current_cost != 0) ? this->convergence_count + 1 : 0;
		if (this->convergence_count == this->convergence_check_number) {
			break;
		}
		this->past_cost = this->current_cost;

		if (this->next_cost > this->current_cost) {
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

	best_cost = this->best_cost;

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
		uniform_int_distribution<int> random_x(0, (DISPLAY_W - this->next_state[this->modify_window_num].getWidth()*0.8));
		this->next_state[this->modify_window_num].setX(random_x(this->mt));
	}
	break;
	case 1:
	{
		uniform_int_distribution<int> random_y(0, (DISPLAY_H - this->next_state[this->modify_window_num].getHeight()*0.8));
		this->next_state[this->modify_window_num].setY(random_y(this->mt));
	}
	break;
	case 2:
	{
		uniform_int_distribution<int> random_w(300, MAX_SUB_WINDOW_W);
		this->next_state[this->modify_window_num].setWidth(random_w(this->mt));
	}
	break;
	case 3:
	{
		uniform_int_distribution<int> random_h(300, MAX_SUB_WINDOW_H);
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
			if (ud.second.state == HandCursor::STATE::INACTIVE) {
				continue;
			}
			if (s.second.intersects(ofRectangle(ofClamp(ud.second.transformed_cursor_point.x - USER_CERTAIN_WINDOW.getX(), 0, DISPLAY_W), ofClamp(ud.second.transformed_cursor_point.y - USER_CERTAIN_WINDOW.getY(), 0, DISPLAY_H), USER_CERTAIN_WINDOW.getWidth(), USER_CERTAIN_WINDOW.getHeight()))) {
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
			if (s.second.intersects(s2.second)) { // もし重複していたら、コストを最大にしてコスト計算を終了
				this->next_cost = 0.0;
				return;
			}
		}

		this->next_cost -= s.second.getArea(); // 自分との重複面積分減らす

		/* 矩形と顔との距離 */
		for (const auto& td : this->hc->user_data) {
			if ((*this->sub_windows)[s.first].get_user_id() == td.first) {
				this->distance_cost = max(this->distance_cost, static_cast<double>((s.second.getCenter().x - td.second.transformed_face_point.x) * (s.second.getCenter().x - td.second.transformed_face_point.x) + (s.second.getCenter().y - td.second.transformed_face_point.y) * (s.second.getCenter().y - td.second.transformed_face_point.y)));
				continue;
			}
		}
	}

	this->area_cost = -min_element(begin(this->next_state), end(this->next_state), [](const pair<int, ofRectangle>& a, const pair<int, ofRectangle>& b) {return a.second.getArea() < b.second.getArea(); })->second.getArea();

	this->next_cost += 150 * this->area_cost + this->overlap_cost + this->distance_cost;
}