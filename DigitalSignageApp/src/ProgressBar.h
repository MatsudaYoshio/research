#pragma once
#ifndef ___Class_ProgressBar
#define ___Class_ProgressBar

#include "ofMain.h"

class ProgressBar {
private:
	ofPoint center_point;
	int big_radius, small_radius;
	int granularity;
	int progress_state;
	ofColor color;
	vector<pair<double, double>> weight;
	vector<ofPoint> position;
public:
	void setup(const int &x, const int &y, const int &br, const int &g, const int &sr, const ofColor &c);
	void setup(const ofPoint &p, const int &br, const int &g, const int &sr, const ofColor &c);
	void draw() const;

	void change_state(const int &n);
	void forward_state(const int &n);
	void backward_state(const int &n);
	void reset_state();
	bool is_max_state() const;
};

#endif