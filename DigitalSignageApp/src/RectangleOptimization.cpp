#include "RectangleOptimization.h"

RectangleOptimization::RectangleOptimization(const int &field_width, const int &field_height, double exclude_edge_ratio = 0.1) :field_width(field_width), field_height(field_width) {
	this->histogram = vector<vector<int>>(field_height, vector<int>(field_width, 1));

	for (int y = 0; y < field_height; ++y) {
		for (int x = 0; x < field_width; ++x) {
			if (x <= field_width*exclude_edge_ratio || x >= field_width*(1 - exclude_edge_ratio) || y <= field_height*exclude_edge_ratio || y >= field_height*(1 - exclude_edge_ratio)) {
				this->histogram[y][x] = 0;
			}
		}
	}
}

void RectangleOptimization::add_block(ofRectangle rect, double exclude_edge_ratio = 0.1) {
	rect.set(max((int)(rect.x - rect.width*exclude_edge_ratio), 0), max((int)(rect.y - rect.height*exclude_edge_ratio), 0), rect.width*(1 + 2 * exclude_edge_ratio), rect.height*(1 + 2 * exclude_edge_ratio));

	for (int y = 0; y < this->field_height; ++y) {
		for (int x = 0; x < this->field_width; ++x) {
			if (rect.inside(ofPoint(x, y))) {
				this->histogram[y][x] = 0;
			}
		}
	}
}

void RectangleOptimization::add_block(const ofPoint &p, int exclude_pixel = 50) {
	for (int y = 0; y < this->field_height; ++y) {
		for (int x = 0; x < this->field_width; ++x) {
			if (x >= p.x - exclude_pixel && x <= p.x + exclude_pixel && y >= p.y - exclude_pixel && y <= p.y + exclude_pixel) {
				this->histogram[y][x] = 0;
			}
		}
	}
}

void RectangleOptimization::calculate() {
	for (int y = 1; y < this->field_height; ++y) {
		for (int x = 0; x < this->field_width; ++x) {
			if (this->histogram[y][x] != 0) {
				this->histogram[y][x] = this->histogram[y - 1][x] + 1;
			}
		}
	}

	for (int y = 0; y < this->field_height; ++y) {
		stack<pair<int, int>> s;
		for (int x = 0; x < this->field_width; ++x) {
			if (s.empty()) {
				s.push(make_pair(this->histogram[y][x], x));
			}
			else if (s.top().first < this->histogram[y][x]) {
				s.push(make_pair(this->histogram[y][x], x));
			}
			else if (s.top().first > this->histogram[y][x]) {
				while (!s.empty() && s.top().first >= this->histogram[y][x]) {
					this->rects.emplace_back(ofRectangle(max(s.top().second, 0), max(y - s.top().first, 0), max(x - s.top().second, 1), max(s.top().first, 1)));
					s.pop();
				}
				s.push(make_pair(this->histogram[y][x], x));
			}
		}
		while (!s.empty()) {
			this->rects.emplace_back(ofRectangle(max(s.top().second, 0), max(y - s.top().first, 0), max(this->field_width - s.top().second, 1), max(s.top().first, 1)));
			s.pop();
		}
	}
}

vector<ofRectangle> RectangleOptimization::get_rects() const {
	return this->rects;
}

ofRectangle RectangleOptimization::get_max_area_rect() const {
	return *max_element(begin(this->rects), end(this->rects), [](const ofRectangle &r1, const ofRectangle &r2) {return r1.getArea() < r2.getArea(); });
}