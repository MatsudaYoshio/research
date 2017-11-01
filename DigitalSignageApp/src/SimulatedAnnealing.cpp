#include "SimulatedAnnealing.h"

using namespace param;

void SimulatedAnnealing::setup(HandCursor* hc, vector<int>* active_window_list, vector<int>* main_window_user_list, unordered_map<int, SubWindow>* sub_windows) {
	this->hc = hc;
	this->active_window_list = active_window_list;
	this->main_window_user_list = main_window_user_list;
	this->sub_windows = sub_windows;
}

void SimulatedAnnealing::operator() (const unordered_map<int, ofRectangle>& start_state, unordered_map<int, ofRectangle>& best_state) {
	this->active_window_list = active_window_list;
	this->current_state = start_state;

	this->current_cost = this->next_cost = this->best_cost = DBL_MAX;

	for (int i = 0; i < this->MAX_ITERATION; ++i) {
		if (!this->set_next_state()) {
			continue;
		}
		this->calculate_cost();

		if (this->next_cost > this->current_cost) {
			double diff = this->current_cost - this->next_cost;
			double t = (double)i / this->MAX_ITERATION;
			if (ofRandomuf() < exp(diff / t)) {
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
}

bool SimulatedAnnealing::set_next_state() {
	this->next_state = this->current_state;

	this->modify_window_num = this->active_window_list->at(ofRandom(0, this->active_window_list->size())); // 修正するウィンドウをランダムに決める

	this->modify_param = ofRandom(0, 4); // 修正するパラメータをランダムに決める
	switch (this->modify_param) {
	case 0:
		this->next_state[modify_window_num].setX(ofRandom(0, W - this->next_state[modify_window_num].getWidth()));
		break;
	case 1:
		this->next_state[modify_window_num].setY(ofRandom(0, H - this->next_state[modify_window_num].getHeight()));
		break;
	case 2:
		this->next_state[modify_window_num].setWidth(ofRandom(30, W / 2));
		break;
	case 3:
		this->next_state[modify_window_num].setHeight(ofRandom(30, H / 2));
		break;
	}

	for (const auto &s : this->next_state) {
		if (s.second.getLeft() < 0.01*W || s.second.getRight() > 0.99*W || s.second.getTop() < 0.01*H || s.second.getBottom() > 0.99*H || s.second.width > W / 2 || s.second.height > H / 2) {
			return false;
		}
	}

	return true;
}

void SimulatedAnnealing::calculate_cost() {
	this->next_cost = 0.0;

	constexpr double a = 100;
	constexpr double b = 1.6;
	constexpr double c = 1000;

	double area_sum = 0.0;

	for (const auto &s : this->next_state) {
		area_sum += s.second.getArea();

		double aspect_ratio = s.second.width / s.second.height;
		if (1.0 / b < aspect_ratio && aspect_ratio < b) {
			this->next_cost += a*exp(-pow(aspect_ratio - ((b*b + 1) / (2 * b)), 2));
		}
		else {
			this->next_cost += c;
		}

		this->next_cost -= 4000 * s.second.getArea();

		for (const auto &id : *this->main_window_user_list) {
			if (s.second.inside(this->hc->track_data[id].current_pointer.x, this->hc->track_data[id].current_pointer.y)) {
				this->next_cost += 100000;
			}
			this->next_cost -= 5000 * this->euclid_distance(s.second.x, s.second.y, this->hc->track_data[id].current_pointer.x, this->hc->track_data[id].current_pointer.y);
		}

		for (const auto &td : this->hc->track_data) {
			if (this->sub_windows->at(s.first).get_user_id() == td.first) {
				this->next_cost += 10 * this->euclid_distance(s.second.x, s.second.y, W - td.second.face.left() + td.second.face.width() / 2, td.second.face.top() + td.second.face.height() / 2);
				continue;
			}
		}

		for (const auto &s2 : this->next_state) {
			this->next_cost += 200 * s.second.getIntersection(s2.second).getArea();
		}

	}

	double area_mean = area_sum / this->next_state.size();

	for (const auto &s : this->next_state) {
		this->next_cost += 0.1*pow(area_mean - s.second.getArea(), 2);
	}
}

double SimulatedAnnealing::euclid_distance(const double &x1, const double &y1, const double &x2, const double &y2) const {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}