#include <opencv2/opencv.hpp>

#include <cmath>
#include <vector>
#include <utility>
#include <algorithm>

#include <dlib/opencv.h>

#include "SlidingWindows.h"

using namespace std;
using namespace dlib;

SlidingWindows::SlidingWindows(const int &window_size, const int &step, const int &min_width, const int &max_width, const int &min_height, const int &max_height) :window_size(window_size), step(step), min_width(min_width), max_width(max_width), min_height(min_height), max_height(max_height) {
	for (int i = this->min_height; i < this->max_height - this->window_size; i += this->step) {
		for (int j = this->min_width; j < this->max_width - this->window_size; j += this->step) {
			windows.emplace_back(rectangle(j, i, j + this->window_size, i + this->window_size));
		}
	}
}

const std::vector<rectangle>& SlidingWindows::get_windows() {
	return this->windows;
}

void SlidingWindows::center_sort() {
	this->center_point = make_pair((this->min_width + this->max_width) / 2, (this->min_height + this->max_height) / 2);

	sort(begin(this->windows), end(this->windows), [this](const rectangle& w1, const rectangle& w2) {return this->center_distance(w1) < this->center_distance(w2); });
}

double SlidingWindows::euclidean_distance(const double &x1, const double &y1, const double &x2, const double &y2) const {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

double SlidingWindows::center_distance(const rectangle& window) const {
	return this->euclidean_distance((window.left() + window.right()) / 2, (window.top() + window.bottom()) / 2, this->center_point.first, this->center_point.second);
}