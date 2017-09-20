#pragma once
#ifndef ___Class_BaseScene
#define ___Class_BaseScene

#include "ofMain.h"
#include "HandPointer.h"
#include "Icon.h"

class BaseScene {
public:
	static constexpr int window_width = 1920;
	static constexpr int window_height = 1080;

	virtual void setup() {};
	virtual void setup(HandPointer *hp) {};
	virtual void update() {};
	virtual void draw() {};
	virtual void change_icon_state (string icon_id, string state) {};
	virtual void set_icon_pointer_id(string icon_id, int pointer_id) {};

	vector<int> pointer_id;
	ofEvent<pair<string,int>> point_event;
	ofEvent<int> transition_event;
	ofEvent<int> make_sub_window_event;
	unordered_map<string, Icon> icons;
};

#endif