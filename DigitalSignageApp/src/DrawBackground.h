#pragma once
#ifndef ___Class_DrawBackground
#define ___Class_DrawBackground

#include "ofMain.h"

class DrawBackground{
private:
	pair<ofColor, ofColor> color_grad[100];
	int n;
public:
	void setup(const ofColor& color1, const ofColor& color2);
	void draw();
};

#endif