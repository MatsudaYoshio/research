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
	void setup(int x, int y, int br, int g, int sr, const ofColor& c);
	void setup(const ofPoint& p, int br, int g, int sr, const ofColor& c);
	void draw() const;

	void change_state(int n);
	void forward_state(int n);
	void backward_state(int n);
	void reset_state();
	bool is_max_state() const;
};

#endif