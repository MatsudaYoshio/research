#pragma once
#ifndef ___Class_MenuBar
#define ___Class_MenuBar

#include "AppParameters.h"
#include "MenuItem.h"

class MenuBar {
private:
	static constexpr int item_num{ 4 };
	array<MenuItem, item_num> items;
public:
	void setup();
	void update();
	void draw();
};

#endif