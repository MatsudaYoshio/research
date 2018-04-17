#pragma once
#ifndef ___Class_OneEuroFilter
#define ___Class_OneEuroFilter

#define _USE_MATH_DEFINES
#include "math.h"
#include <stdexcept>
#include <memory>

#include "LowPassFilter.h"

class OneEuroFilter {
private:
	double freq, mincutoff, beta_, dcutoff;
	std::unique_ptr<LowPassFilter> x, dx;

	double alpha(double cutoff) const;
public:
	explicit OneEuroFilter(double freq, double mincutoff = 1.0, double beta_ = 0.0, double dcutoff = 1.0);
	double filter(double value);
};

#endif