#pragma once
#ifndef ___Class_Pin
#define ___Class_Pin

#include "ofMain.h"
#include "AppParameters.h"

class Pin {
private:
	param::MENU_ITEM_ID item_id;
public:
	Pin(param::MENU_ITEM_ID item_id);
	void update();
	void draw() const;
};

#endif