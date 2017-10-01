#pragma once
#ifndef ___Class_OptimizedForm
#define ___Class_OptimizedForm

#include <vector>

#include "ofMain.h"

class OptimizedForm {
private:
	vector<vector<int>> field;
	vector<vector<ofRectangle>> all_rects;
	vector<ofRectangle> active_rects;
	vector<ofPoint> cursors;
	double past_cost, current_cost;
public:
	OptimizedForm();
	OptimizedForm(const int &w, const int &h, const int &split_x, const int &split_y);
	void setup(const int &w, const int &h, const int &split_x, const int &split_y);
	void update(const ofPoint &p);
	void draw() const;
private:
	void optimization();
	double calculate_cost(const vector<ofRectangle> &active_rects) const;
	double euclid_distance(const double &x1, const double &y1, const double &x2, const double &y2) const;
	double euclid_distance(const ofPoint &p1, const ofPoint &p2) const;
};

#endif