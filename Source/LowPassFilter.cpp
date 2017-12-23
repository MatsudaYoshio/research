#include "LowPassFilter.h"

LowPassFilter::LowPassFilter(const double& alpha, const double& initval) :y(initval), s(initval), initialized(false) {
	this->set_alpha(alpha);
}

void LowPassFilter::set_alpha(const double& alpha) {
	if (alpha <= 0.0 || alpha > 1.0) {
		throw std::range_error("alpha should be in (0.0., 1.0]");
	}
	this->a = alpha;
}

double LowPassFilter::filter(const double& value) {
	this->y = value;
	if (this->initialized) {
		return this->s = this->a*value + (1.0 - this->a)*this->s;
	}
	else {
		this->initialized = true;
		return this->s = value;
	}
}

double LowPassFilter::filter_with_alpha(const double& value, const double& alpha) {
	this->set_alpha(alpha);
	return this->filter(value);
}

bool LowPassFilter::has_last_raw_value() const {
	return this->initialized;
}

double LowPassFilter::get_last_raw_value() const {
	return this->y;
}