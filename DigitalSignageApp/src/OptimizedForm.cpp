#include "OptimizedForm.h"

OptimizedForm::OptimizedForm() {

}

OptimizedForm::OptimizedForm(const int &w, const int &h, const int &split_x, const int &split_y) {
	this->setup(w, h, split_x, split_y);
}

void OptimizedForm::setup(const int &w, const int &h, const int &split_x, const int &split_y) {
	this->split_x = split_x;
	this->split_y = split_y;

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
		ofSetColor(ofColor::blue);
		ofRect(r);
	}
}

void OptimizedForm::optimization() {
	this->active_rects_tmp = this->active_rects;
	this->past_cost = this->best_cost = DBL_MAX;

	int modify_rect_num;
	ofRectangle modify_rect, new_rect;

	for (int i = 1; i <= this->MAX_ITERATION; ++i) {
		modify_rect_num = ofRandom(0, this->active_rects_tmp.size());
		modify_rect = this->active_rects_tmp[modify_rect_num];
		this->active_rects_tmp.erase(begin(this->active_rects_tmp) + modify_rect_num);
		new_rect = this->all_rects[ofRandom(0, this->all_rects.size())][ofRandom(0, this->all_rects[0].size())];
		this->active_rects_tmp.emplace_back(new_rect);

		this->field[modify_rect.y / this->split_y][modify_rect.x / this->split_x] = max(this->field[modify_rect.y / this->split_y][modify_rect.x / this->split_x] - 1, 0);
		++this->field[new_rect.y / this->split_y][new_rect.x / this->split_x];

		this->current_cost = this->calculate_cost(active_rects_tmp);

		if (this->current_cost > this->past_cost) {
			if (ofRandomuf() > exp((this->past_cost - this->current_cost) * (double)this->MAX_ITERATION / (double)i)) {
				this->active_rects_tmp.erase(end(this->active_rects_tmp) - 1);
				this->active_rects_tmp.emplace_back(modify_rect);
				--this->field[new_rect.y / this->split_y][new_rect.x / this->split_x];
				++this->field[modify_rect.y / this->split_y][modify_rect.x / this->split_x];
			}
			else {
				this->past_cost = this->current_cost;
			}
		}
		else {
			this->past_cost = this->current_cost;

			if (this->current_cost < this->best_cost) {
				this->best_rects = this->active_rects_tmp;
				this->best_cost = this->current_cost;
			}
		}
	}

	this->active_rects = this->best_rects;
}

double OptimizedForm::calculate_cost(const vector<ofRectangle> &active_rects) const {
	double cost = 0.0;

	double cursor_distance = 0.0;
	for (const auto &r : active_rects) {
		for (const auto &c : this->cursors) {
			cursor_distance -= this->euclid_distance(c, r.getCenter());
		}
	}

	int dx[] = { -1,0,1,0 }, dy[] = { 0,-1,0,1 };
	double four_neighborhood = 0.0;
	for (const auto &r : active_rects) {
		for (int i = 0; i < 4; ++i) {
			int next_y = r.y / this->split_y + dy[i], next_x = r.x / this->split_x + dx[i];
			if (next_x < 0 || next_x >= this->field[0].size() || next_y < 0 || next_y >= this->field.size()) {
				continue;
			}
			if (this->field[next_y][next_x] > 0) {
				four_neighborhood -= 200;
			}
		}
	}

	cost += cursor_distance + four_neighborhood;

	return cost;
}

double OptimizedForm::euclid_distance(const double &x1, const double &y1, const double &x2, const double &y2) const {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

double OptimizedForm::euclid_distance(const ofPoint &p1, const ofPoint &p2) const {
	return this->euclid_distance(p1.x, p1.y, p2.x, p2.y);
}