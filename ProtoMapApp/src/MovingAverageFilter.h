#pragma once
#ifndef ___Class_MovingAverageFilter
#define ___Class_MovingAverageFilter

#include <queue>

class MovingAverageFilter {
private:
	std::queue<double> data;
	double sum, filtered_x;
	int n;
public:
	MovingAverageFilter(const int n);
	double filter(const double x);
};

#endif