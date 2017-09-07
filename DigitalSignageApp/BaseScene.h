#pragma once
#ifndef ___Class_BaseScene
#define ___Class_BaseScene

#include "ofMain.h"
#include "HandPointer.h"
#include "Icon.h"

class BaseScene {
public:
	virtual void setup() {};
	virtual void setup(HandPointer *hp) {};
	virtual void update() {};
	virtual void draw() {};

	ofEvent<pair<string,int>> point_event;
	unordered_map<string, Icon> icons;
};

#endif