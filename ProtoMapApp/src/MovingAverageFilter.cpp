#include "MovingAverageFilter.h"

MovingAverageFilter::MovingAverageFilter(int n) {
	this->n = std::max(n, 1);
}

double MovingAverageFilter::filter(double x) {
	this->sum += x;
	this->data.emplace(x);

	this->filtered_x = this->sum / this->data.size();

	if (this->data.size() >= this->n) {
		this->sum -= this->data.front();
		this->data.pop();
	}

	return this->filtered_x;
}