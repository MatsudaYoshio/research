#include "OptimizedForm.h"

OptimizedForm::OptimizedForm() {

}

OptimizedForm::OptimizedForm(const int &w, const int &h, const int &split_x, const int &split_y) {
	this->setup(w, h, split_x, split_y);
}

void OptimizedForm::setup(const int &w, const int &h, const int &split_x, const int &split_y) {
	this->field = vector<vector<int>>(h / split_y, vector<int>(w / split_x, 0));
	this->all_rects = vector<vector<ofRectangle>>(h / split_y, vector<ofRectangle>(w / split_x));

	for (int x = 0, x_pos = 0; x < this->all_rects[0].size(); ++x, x_pos += split_x) {
		for (int y = 0, y_pos = 0; y < this->all_rects.size(); ++y, y_pos += split_y) {
			this->all_rects[y][x] = ofRectangle(x_pos, y_pos, split_x, split_y);
		}
	}
	
	for (int x = 10; x <= 50; ++x) {
		for (int y = 10; y <= 50; ++y) {
			this->field[y][x] = 1;
			this->active_rects.emplace_back(all_rects[y][x]);
		}
	}
}

void OptimizedForm::update(const ofPoint &p) {
	this->cursors.clear();
	this->cursors.emplace_back(p);

	this->optimization();
}

void OptimizedForm::draw() const {
	for (auto r : this->active_rects) {
		ofSetColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));
		ofRect(r);
	}
}

void OptimizedForm::optimization() {
	vector<ofRectangle> active_rects_tmp = this->active_rects, best_rects;
	double best_cost = DBL_MAX;
	this->past_cost = DBL_MAX;
	
	for (int i = 0; i < 100; ++i) {
		int modify_rect_num = ofRandom(0, active_rects_tmp.size());
		ofRectangle modify_rect = active_rects_tmp[modify_rect_num];
		active_rects_tmp.erase(begin(active_rects_tmp) + modify_rect_num);
		active_rects_tmp.emplace_back(this->all_rects[ofRandom(0, this->all_rects.size())][ofRandom(0, this->all_rects[0].size())]);

		this->current_cost = this->calculate_cost(active_rects_tmp);

		cout << this->current_cost << endl;

		if (this->current_cost > this->past_cost && ofRandom(0, i + 2) == 0) {
			active_rects_tmp.erase(end(active_rects_tmp) - 1);
		}
		else {
			this->past_cost = this->current_cost;

			if (this->current_cost < best_cost) {
				best_rects = active_rects_tmp;
			}
		}
	}

	this->active_rects = best_rects;
}

double OptimizedForm::calculate_cost(const vector<ofRectangle> &active_rects) const {
	double cost = 0.0;

	double cursor_distance = 0.0;
	for (auto r : active_rects) {
		for (auto c : this->cursors) {
			cursor_distance += this->euclid_distance(c, r.getCenter());
		}
	}

	double four_neighborhood = 0.0;
	for (auto r : active_rects) {

	}

	cost -= cursor_distance;

	return cost;
}

double OptimizedForm::euclid_distance(const double &x1, const double &y1, const double &x2, const double &y2) const {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

double OptimizedForm::euclid_distance(const ofPoint &p1, const ofPoint &p2) const {
	return this->euclid_distance(p1.x, p1.y, p2.x, p2.y);
}