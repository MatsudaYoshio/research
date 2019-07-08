#include "OneEuroFilter.h"

OneEuroFilter::OneEuroFilter(const double freq, const double mincutoff, const double beta_, const double dcutoff) :freq(freq), mincutoff(mincutoff), beta_(beta_), dcutoff(dcutoff) {
	this->x.reset(new LowPassFilter(this->alpha(mincutoff)));
	this->dx.reset(new LowPassFilter(this->alpha(dcutoff)));
}

double OneEuroFilter::filter(const double value) {
	return x->filter_with_alpha(value, this->alpha(this->mincutoff + this->beta_*fabs(this->dx->filter_with_alpha(this->x->has_last_raw_value() ? (value - this->x->get_last_raw_value())*this->freq : 0.0, this->alpha(this->dcutoff)))));
}

double OneEuroFilter::alpha(const double cutoff) const {
	return 1.0 / (1.0 + (this->freq / (2 * M_PI*cutoff)));
}