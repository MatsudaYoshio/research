#pragma once
#ifndef ___Class_RectangleOptimization
#define ___Class_RectangleOptimization

#include "ofMain.h"

class RectangleOptimization {
private:
	int field_width, field_height;
	vector<vector<int>> histogram;
	vector<ofRectangle> rects;
public:
	RectangleOptimization(const int &field_width, const int &field_height, double exclude_edge_ratio = 0.1);
	void add_block(ofRectangle rect, double exclude_edge_ratio = 0.1);
	void add_block(const ofPoint &p, int exclude_pixel = 50);
	void calculate();
	vector<ofRectangle> get_rects() const;
	ofRectangle get_max_area_rect() const;
};

#endif