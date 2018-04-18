#pragma once
#ifndef ___Class_SceneManager
#define ___Class_SceneManager

#include "AppParameters.h"
#include "MenuBar.h"
#include "AdBar.h"
#include "HandCursor.h"
#include "Pin.h"

class SceneManager {
private:
	HandCursor* hc;
	MenuBar mb;
	AdBar ab;
	const ofImage map_image{ "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/kyoto_bg.png" };
	array<bool, param::MENU_ITEM_NUM> menu_item_flag;
	array<vector<Pin>, param::MENU_ITEM_NUM> pins;
public:
	void setup(HandCursor* const hc);
	void update();
	void draw();

	void add_pin(param::MENU_ITEM_ID& item_id);
	void make_sub_window(param::CONTENT_ID& content_id);

	~SceneManager();
};

#endif