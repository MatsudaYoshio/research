#pragma once
#ifndef ___Class_LowPassFilter
#define ___Class_LowPassFilter

#include <stdexcept>

class LowPassFilter {
private:
	double y, a, s;
	bool initialized;

	void set_alpha(double alpha);
	double filter(double value);
public:
	explicit LowPassFilter(double alpha, double initval = 0.0);
	double filter_with_alpha(double value, double alpha);
	bool has_last_raw_value() const;
	double get_last_raw_value() const;
};

#endif