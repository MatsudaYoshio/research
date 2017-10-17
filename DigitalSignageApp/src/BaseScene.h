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

	virtual void select_icon(const string &icon_id, const int &user_id) {};

	vector<int> pointer_id;
	ofEvent<pair<string,int>> point_event;
	ofEvent<int> transition_event;
	ofEvent<int> make_sub_window_event;
	unordered_map<string, Icon> icons;
};

#endif