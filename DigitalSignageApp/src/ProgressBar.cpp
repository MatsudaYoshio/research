#include "ProgressBar.h"

void ProgressBar::setup(const int &x, const int &y, const int &br, const int &g, const int &sr, const ofColor &c) {
	this->setup(ofPoint(x, y), br, g, sr, c);
}

void ProgressBar::setup(const ofPoint &p, const int &br, const int &g, const int &sr, const ofColor &c) {
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
		this->position[i] = ofPoint(this->weight[i].first*br, this->weight[i].second*br);
	}
}

void ProgressBar::draw() const {
	ofTranslate(this->center_point);
	ofSetColor(this->color, 255);
	for (int i = 1; i <= this->progress_state; ++i) {
		ofDrawCircle(this->position[i-1], this->small_radius);
	}
	ofSetColor(this->color, 50);
	for (int i = this->progress_state+1; i <= this->granularity; ++i) {
		ofDrawCircle(this->position[i-1], this->small_radius);
	}
}

void ProgressBar::change_state(const int &n) {
	this->progress_state = min(max(n, 0), this->granularity);
}

void ProgressBar::forward_state(const int &n) {
	this->change_state(this->progress_state + n);
}