#pragma once
#ifndef ___Class_WhacAMoleApp
#define ___Class_WhacAMoleApp

#include "AppParameters.h"

class WhacAMoleApp : public ofBaseApp {
public:
	static const ofColor bg_color;

	const ofImage hammer_image{ "fig/hammer.png" };

	void setup();
	void update();
	void draw();
};

#endif