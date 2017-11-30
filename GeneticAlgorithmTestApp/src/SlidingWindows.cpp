#include <opencv2/opencv.hpp>

#include <cmath>
#include <vector>
#include <utility>
#include <algorithm>

#include <dlib/opencv.h>

#include "SlidingWindows.h"

using namespace std;
using namespace dlib;

SlidingWindows::SlidingWindows(const int& window_size, const int& step, const int& min_width, const int& max_width, const int& min_height, const int& max_height) :window_size(window_size), step(step), min_width(min_width), max_width(max_width), min_height(min_height), max_height(max_height) {
	const int max_i = max_height - window_size;
	const int max_j = max_width - window_size;
	this->windows.resize(((max_i - min_height) / step + 1)*((max_j - min_width) / step + 1));

	for (int i = min_height, x = 0; i <= max_i; i += step) {
		for (int j = min_width; j <= max_j; j += step) {
			this->windows[x++] = dlib::rectangle(j, i, j + window_size, i + window_size);
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

double SlidingWindows::euclidean_distance(const double& x1, const double& y1, const double& x2, const double& y2) const {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

double SlidingWindows::center_distance(const rectangle& window) const {
	return this->euclidean_distance((window.left() + window.right()) / 2, (window.top() + window.bottom()) / 2, this->center_point.first, this->center_point.second);
}