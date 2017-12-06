#include "ProgressBar.h"

void ProgressBar::setup(const int& x, const int& y, const int& br, const int& g, const int& sr, const ofColor& c) {
	this->setup(ofPoint(x, y), br, g, sr, c);
}

void ProgressBar::setup(const ofPoint& p, const int& br, const int& g, const int& sr, const ofColor& c) {
	this->progress_state = 0;

	this->center_point.set(p);
	this->big_radius = br;
	this->small_radius = sr;
	this->granularity = g;
	this->color = c;

	this->weight.resize(g);
	this->position.resize(g);
	double theta;
	for (int i = 0; i < g; ++i) {
		theta = i * 2 * PI / g - PI / 2;
		this->weight[i] = make_pair(cos(theta), sin(theta));
		this->position[i] = ofPoint(this->weight[i].first*br + this->center_point.x, this->weight[i].second*br + this->center_point.y);
	}
}

void ProgressBar::draw() const {
	ofSetColor(this->color, 255);
	for (int i = 1; i <= this->progress_state; ++i) {
		ofDrawCircle(this->position[i - 1], this->small_radius);
	}
	ofSetColor(this->color, 50);
	for (int i = this->progress_state + 1; i <= this->granularity; ++i) {
		ofDrawCircle(this->position[i - 1], this->small_radius);
	}
}

void ProgressBar::change_state(const int& n) {
	this->progress_state = ofClamp(n, 0, this->granularity);
}

void ProgressBar::forward_state(const int& n) {
	this->change_state(this->progress_state + n);
}

void ProgressBar::backward_state(const int& n) {
	this->change_state(this->progress_state - n);
}

void ProgressBar::reset_state() {
	this->change_state(0);
}

bool ProgressBar::is_max_state() const {
	return this->progress_state == this->granularity;
}