#pragma once
#ifndef ___Class_AdItem
#define ___Class_AdItem

#include "AppParameters.h"
#include "DrawFrame.h"

class AdItem {
private:
	DrawFrame df;
	int x, y;
public:
	void setup(int x, int y);
	void update();
	void draw();
};

#endif