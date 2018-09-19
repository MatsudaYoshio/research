#pragma once
#ifndef ___Class_Mole
#define ___Class_Mole

#include "ofMain.h"

#include "AppParameters.h"

class Mole {
private:
	ofRectangle draw_rect;
public:
	void setup(const ofRectangle& draw_rect);
	void update();
	void draw();
};

#endif