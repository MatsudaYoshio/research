#pragma once
#ifndef ___Class_LowPassFilter
#define ___Class_LowPassFilter

#include <stdexcept>

class LowPassFilter {
private:
	double y, a, s;
	bool initialized;

	void set_alpha(const double& alpha);
	double filter(const double& value);
public:
	LowPassFilter(const double& alpha, const double& initval = 0.0);
	double filter_with_alpha(const double& value, const double& alpha);
	bool has_last_raw_value() const;
	double get_last_raw_value() const;
};

#endif