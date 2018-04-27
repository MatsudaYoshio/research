#pragma once
#ifndef ___Class_SceneManager
#define ___Class_SceneManager

#include "AppParameters.h"
#include "MenuBar.h"
#include "AdBar.h"
#include "HandCursor.h"
#include "Pin.h"
#include "SubWindow.h"
#include "SimulatedAnnealing.h"

class SceneManager {
private:
	HandCursor* hc;
	MenuBar mb;
	AdBar ab;
	SimulatedAnnealing sa;

	bool transform_thread_flag{ false };
	const ofImage map_image{ "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/kyoto_bg.png" };
	array<bool, param::MENU_ITEM_NUM> menu_item_flag;
	array<vector<Pin>, param::MENU_ITEM_NUM> pins;
	unordered_map<long long int, SubWindow> sub_windows;
	unordered_map<long long int, ofRectangle> rects_tmp, old_rects, best_rects;
public:
	void setup(HandCursor* const hc);
	void update();
	void draw();

	void transform(unordered_map<long long int, ofRectangle>& old_rects, unordered_map<long long int, ofRectangle>& new_rects);
	void add_pin(param::MENU_ITEM_ID& item_id);
	void make_sub_window(pair<param::CONTENT_ID, long long int>& id);

	~SceneManager();
};

#endif