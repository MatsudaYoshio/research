#pragma once
#ifndef ___Class_MainScene
#define ___Class_MainScene

#include "ofMain.h"
#include "HandCursor.h"
#include "DrawBackground.h"
#include "Icon.h"

class MainScene {
private:
	HandCursor* hc;
	DrawBackground db;
	unordered_map<string, Icon> icons;
	static constexpr int curve_vertices_num = 7;
	vector<ofPoint> curve_vertices;
public:
	vector<int> user_id_list;
	ofEvent<pair<string, int>> point_event;
	ofEvent<int> make_sub_window_event;

	void setup(HandCursor *hc);
	void update();
	void draw();

	void select_icon(int &pointer_id);
	void point_icon(const string &icon_id, const int &user_id);
};

#endif