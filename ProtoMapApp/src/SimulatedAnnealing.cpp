#include "SimulatedAnnealing.h"

using namespace param;

random_device SimulatedAnnealing::rd;
mt19937 SimulatedAnnealing::mt(SimulatedAnnealing::rd());
uniform_int_distribution<int> SimulatedAnnealing::random_parameter(0, 3);
uniform_real_distribution<double> SimulatedAnnealing::random_0to1(0.0, 1.0);

array<double, SimulatedAnnealing::MAX_ITERATION> SimulatedAnnealing::kT;
const double SimulatedAnnealing::a{ 5 / (sqrt(2 * PI)*sigma) };

void SimulatedAnnealing::setup(HandCursor* hc, unordered_map<long long int, SubWindow>* sub_windows) {
	this->hc = hc;
	this->sub_windows = sub_windows;

	Concurrency::parallel_for(0, MAX_ITERATION, [&](int i) {
		this->kT[i] = this->k*pow(0.00005, static_cast<double>(i) / this->MAX_ITERATION);
	});

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

		if (this->next_cost > this->current_cost) {
			if (this->random_0to1(this->mt) <= exp(-(this->next_cost - this->current_cost) / this->kT[i])) {
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

		/* ポインタとウィンドウの重複 */
		for (const auto& ud : this->hc->user_data) {
			if (ud.second.state == HandCursor::STATE::INACTIVE) {
				continue;
			}

			this->overlap_cost += a*exp(-b*this->minimum_distance(s.second, ud.second.cursor_point.x, ud.second.cursor_point.y));
		}

		/* ウィンドウ間の重複 */
		if (state.size() > 1) {
			for (const auto& s2 : state) {
				if (s.first != s2.first) {
					this->overlap_cost += s.second.getIntersection(s2.second).getArea();
				}
			}
		}

		/* 矩形と顔との距離 */
		for (const auto& td : this->hc->user_data) {
			if (sub_windows.at(s.first).get_user_id() == td.first) {
				this->distance_cost += ofDist(s.second.getCenter().x, s.second.getCenter().y, td.second.transformed_face_point.x, td.second.transformed_face_point.y);
				continue;
			}
		}
	}

	this->area_cost = -min_element(cbegin(state), cend(state), [](const pair<int, ofRectangle>& a, const pair<int, ofRectangle>& b) {return a.second.getArea() < b.second.getArea(); })->second.getArea();

	cost += this->alpha*this->area_cost + this->beta*this->overlap_cost + this->gamma*this->distance_cost;
}

double SimulatedAnnealing::minimum_distance(const ofRectangle& r, const double x, const double y) const {
	const double dx{ max(static_cast<double>(fabs(x - r.getCenter().x) - r.getWidth() / 2), 0.0) };
	const double dy{ max(static_cast<double>(fabs(y - r.getCenter().y) - r.getHeight() / 2), 0.0) };

	return dx*dx + dy*dy;
}
