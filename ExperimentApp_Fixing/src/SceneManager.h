#pragma once
#ifndef ___Class_SceneManager
#define ___Class_SceneManager

#include "MenuBar.h"
#include "HandCursor.h"
#include "Pin.h"
#include "SubWindow.h"

class SceneManager {
private:
	static constexpr int max_menu_item_life{ 80 };
	static constexpr int min_menu_item_life{ 0 };

	HandCursor* hc;
	MenuBar mb;

	const ofImage map_image{ "C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/kyoto_bg.png" };
	array<long long int, param::MENU_ITEM_NUM> menu_item_user_id;
	array<int, param::MENU_ITEM_NUM> menu_item_life;
	array<vector<Pin>, param::MENU_ITEM_NUM> pins;
	unordered_map<long long int, SubWindow> sub_windows, previous_sub_windows;
	unordered_map<long long int, ofRectangle> initial_rects, best_rects, previous_rects;
	double comparative_cost{ 0.0 }, current_cost{ 0.0 }, best_cost{ 0.0 };
	bool make_sub_window_flag{ false };

	void draw_cursor();
public:
	void setup(HandCursor* const hc);
	void update();
	void draw();

	void add_pin(pair<param::MENU_ITEM_ID, long long int>& id);
	void make_sub_window(pair<param::CONTENT_ID, long long int>& id);

	~SceneManager();
};

#endif