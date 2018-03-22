#pragma once
#ifndef ___Class_MenuBar
#define ___Class_MenuBar

#include "AppParameters.h"

class MenuBar {
private:
	array<ofRectangle, 4> icons;
public:
	void setup();
	void update();
	void draw();
};

#endif