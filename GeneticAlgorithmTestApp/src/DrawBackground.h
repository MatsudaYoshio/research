#pragma once
#ifndef ___Class_DrawBackground
#define ___Class_DrawBackground

#include "ofMain.h"

class DrawBackground{
private:
	vector<pair<ofColor, ofColor>> color_grad;
	int n;
	int granularity;
public:
	void setup(const ofColor& color1, const ofColor& color2, const int& g = 100);
	void draw();
};

#endif