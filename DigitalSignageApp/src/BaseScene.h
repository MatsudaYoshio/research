#pragma once
#ifndef ___Class_BaseScene
#define ___Class_BaseScene

#include "ofMain.h"
#include "HandCursor.h"
#include "Icon.h"
#include "AppParameters.h"

class BaseScene {
public:
	virtual void setup() {};
	virtual void setup(HandCursor *hc) {};
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