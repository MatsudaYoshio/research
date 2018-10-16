#include "SimulatedAnnealing.h"

using namespace param;

random_device SimulatedAnnealing::rd;
mt19937 SimulatedAnnealing::mt(SimulatedAnnealing::rd());
uniform_int_distribution<int> SimulatedAnnealing::random_parameter(0, 3);
uniform_real_distribution<double> SimulatedAnnealing::random_0to1(0.0, 1.0);

void SimulatedAnnealing::setup(HandCursor* hc, unordered_map<long long int, SubWindow>* sub_windows) {
	this->hc = hc;
	this->sub_windows = sub_windows;

	//this->cost_plotter.set_range(0, 10000);
	//this->cost_plotter.set_line_color(ofColor::skyBlue);
}

void SimulatedAnnealing::operator() (const unordered_map<long long int, ofRectangle>& initial_state, unordered_map<long long int, ofRectangle>& best_state, double& best_cost) {
	this->next_state = this->current_state = initial_state;

	this->calculate_cost(initial_state, this->next_cost, *this->sub_windows);

	this->current_cost = this->next_cost;

	this->best_cost = this->current_cost;
	best_state = this->current_state;

	//this->ofs.open("cost_data" + to_string(this->file_index) + ".txt");
	for (int i = 0; i < this->MAX_ITERATION; ++i) {
		//this->ofs << this->current_cost << endl;

		this->set_next_state(); // パラメータを制約内で動かして次の状態を決定

		this->calculate_cost(this->next_state, this->next_cost, *this->sub_windows);

		//if (i == 0) {
		//cout << "over lap cost : " << this->overlap_cost << endl;
		//this->cost_plotter.update(this->overlap_cost);
		//this->cost_plotter.draw();
	//}
		double k = 1000000;
		double T = pow(0.00005, static_cast<double>(i) / this->MAX_ITERATION);
		//printf("[%d] : %lf,  %lf\n", i, exp(-(this->next_cost - this->current_cost) / (k * T)), this->next_cost - this->current_cost);
		if (this->next_cost > this->current_cost) {
			//if (this->random_0to1(this->mt) < exp(-1 * this->MAX_ITERATION * log(this->next_cost - this->current_cost) / 10000000 * i)) {
			//if(this->random_0to1(this->mt) <= exp(-1 * this->MAX_ITERATION * (this->next_cost - this->current_cost) / k * i)){
			if (this->random_0to1(this->mt) <= exp(-(this->next_cost - this->current_cost) / (k * T))) {
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

void SimulatedAnnealing::set_next_state() {
	this->next_state = this->current_state;

	auto i{ begin(*this->sub_windows) };
	advance(i, uniform_int_distribution<int>(0, this->sub_windows->size() - 1)(this->mt));

	this->modify_window_num = i->first; // 修正するウィンドウをランダムに決める

	this->modify_param = this->random_parameter(this->mt); // 修正するパラメータをランダムに決める

	switch (this->modify_param) {
	case 0:
		this->next_state[this->modify_window_num].setX(uniform_real_distribution<double>(0, DISPLAY_W - this->next_state[this->modify_window_num].getWidth())(this->mt));
		break;
	case 1:
		this->next_state[this->modify_window_num].setY(uniform_real_distribution<double>(0, DISPLAY_H - this->next_state[this->modify_window_num].getHeight())(this->mt));
		break;
	case 2:
		this->next_state[this->modify_window_num].setWidth(uniform_real_distribution<double>(MIN_SUB_WINDOW_W, min(static_cast<double>(DISPLAY_W) - this->next_state[this->modify_window_num].getX(), static_cast<double>(MAX_SUB_WINDOW_W)))(this->mt));
		break;
	case 3:
		this->next_state[this->modify_window_num].setHeight(uniform_real_distribution<double>(MIN_SUB_WINDOW_H, min(static_cast<double>(DISPLAY_H) - this->next_state[this->modify_window_num].getY(), static_cast<double>(MAX_SUB_WINDOW_H)))(this->mt));
		break;
	}
}

void SimulatedAnnealing::calculate_cost(const unordered_map<long long int, ofRectangle>& state, double& cost, const unordered_map<long long int, SubWindow>& sub_windows) {
	cost = 0.0;

	this->area_cost = 0.0;
	this->overlap_cost = 0.0;
	this->distance_cost = 0.0;

	for (const auto& s : state) {

		for (const auto& ud : this->hc->user_data) {
			if (ud.second.state == HandCursor::STATE::INACTIVE) {
				continue;
			}

			//double sigma = 11000;

			auto center_point = s.second.getCenter();

			//this->overlap_cost += 10000 * exp(-ofDistSquared(center_point.x, center_point.y, ud.second.cursor_point.x, ud.second.cursor_point.y) / (2 * pow(this->sigma, 2)))*s.second.getIntersection(ofRectangle(ofClamp(ud.second.cursor_point.x - USER_CERTAIN_WINDOW.getX(), 0, DISPLAY_W), ofClamp(ud.second.cursor_point.y - USER_CERTAIN_WINDOW.getY(), 0, DISPLAY_H), USER_CERTAIN_WINDOW.getWidth(), USER_CERTAIN_WINDOW.getHeight())).getArea();
			this->overlap_cost += 10000 * exp(-ofDistSquared(center_point.x, center_point.y, ud.second.cursor_point.x, ud.second.cursor_point.y) / (2 * pow(this->sigma, 2)))*s.second.getIntersection(ofRectangle(ofClamp(ud.second.cursor_point.x - USER_CERTAIN_WINDOW.getX(), 0, DISPLAY_W), ofClamp(ud.second.cursor_point.y - USER_CERTAIN_WINDOW.getY(), 0, DISPLAY_H), USER_CERTAIN_WINDOW.getWidth(), USER_CERTAIN_WINDOW.getHeight())).getArea() / (sqrt(2 * PI)*this->sigma);
			//this->overlap_cost += exp(pow(s.second.getIntersection(ofRectangle(ofClamp(ud.second.cursor_point.x - USER_CERTAIN_WINDOW.getX(), 0, DISPLAY_W), ofClamp(ud.second.cursor_point.y - USER_CERTAIN_WINDOW.getY(), 0, DISPLAY_H), USER_CERTAIN_WINDOW.getWidth(), USER_CERTAIN_WINDOW.getHeight())).getArea(), 2) / (2 * pow(sigma, 2))) / (sqrt(2 * PI)*sigma);

			//this->overlap_cost += s.second.getIntersection(ofRectangle(ofClamp(ud.second.cursor_point.x - USER_CERTAIN_WINDOW.getX(), 0, DISPLAY_W), ofClamp(ud.second.cursor_point.y - USER_CERTAIN_WINDOW.getY(), 0, DISPLAY_H), USER_CERTAIN_WINDOW.getWidth(), USER_CERTAIN_WINDOW.getHeight())).getArea();

			//if (s.second.intersects(ofRectangle(ofClamp(ud.second.cursor_point.x - USER_CERTAIN_WINDOW.getX(), 0, DISPLAY_W), ofClamp(ud.second.cursor_point.y - USER_CERTAIN_WINDOW.getY(), 0, DISPLAY_H), USER_CERTAIN_WINDOW.getWidth(), USER_CERTAIN_WINDOW.getHeight()))) {
			//	// もし矩形とカーソルの周辺矩形が重複していたら、コストを最大にしてコスト計算を終了
			//	//this->overlap_cost += 50000000000;
			//	cost = 0.0;
			//	return;
			//}
		}

		if (state.size() != 1) {
			for (const auto& s2 : state) {
				//this->overlap_cost += s.second.getIntersection(s2.second).getArea();
				if (s.first != s2.first) {
					this->overlap_cost += 1555200 * (atan(s.second.getIntersection(s2.second).getArea()) / PI + 0.5);
					//this->overlap_cost += exp(pow(s.second.getIntersection(s2.second).getArea(), 2) / (2 * pow(this->sigma, 2))) / (sqrt(2 * PI)*this->sigma);
				}
			}

			//for (const auto& s2 : state) {
			//	if (s.second == s2.second) { // 自分との重複面積は除く
			//		continue;
			//	}
			//	if (s.second.intersects(s2.second)) { // もし重複していたら、コストを最大にしてコスト計算を終了
			//		//this->overlap_cost += 50000000000;
			//		cost = 0.0;
			//		return;
			//	}
			//}

			//this->overlap_cost -= exp(pow(s.second.getArea(), 2) / (2 * pow(this->sigma, 2))) / (sqrt(2 * PI)*this->sigma);

			//this->overlap_cost -= s.second.getArea(); // 自分との重複面積分減らす
		}

		/* 矩形と顔との距離 */
		for (const auto& td : this->hc->user_data) {
			if (sub_windows.at(s.first).get_user_id() == td.first) {
				//this->distance_cost = max(this->distance_cost, static_cast<double>((s.second.getCenter().x - td.second.transformed_face_point.x) * (s.second.getCenter().x - td.second.transformed_face_point.x) + (s.second.getCenter().y - td.second.transformed_face_point.y) * (s.second.getCenter().y - td.second.transformed_face_point.y)));
				this->distance_cost = max(this->distance_cost, static_cast<double>(sqrt((s.second.getCenter().x - td.second.transformed_face_point.x) * (s.second.getCenter().x - td.second.transformed_face_point.x) + (s.second.getCenter().y - td.second.transformed_face_point.y) * (s.second.getCenter().y - td.second.transformed_face_point.y))));
				continue;
			}
		}
	}

	//this->overlap_cost = exp(pow(this->overlap_cost, 2) / (2 * pow(this->sigma, 2))) / (sqrt(2 * PI)*this->sigma);

	this->area_cost = -min_element(begin(state), end(state), [](const pair<int, ofRectangle>& a, const pair<int, ofRectangle>& b) {return a.second.getArea() < b.second.getArea(); })->second.getArea();

	cost += this->area_cost + this->overlap_cost + this->distance_cost;
}