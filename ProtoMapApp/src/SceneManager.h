#pragma once
#ifndef ___Class_SceneManager
#define ___Class_SceneManager

#include "MenuBar.h"
#include "HandCursor.h"
#include "Pin.h"
#include "SubWindow.h"
#include "SimulatedAnnealing.h"

class SceneManager {
private:
	static constexpr int max_menu_item_life{ 80 };
	static constexpr int min_menu_item_life{ 0 };

	HandCursor* hc;
	MenuBar mb;
	SimulatedAnnealing sa;

	bool transform_thread_flag{ false };
	const ofImage map_image{ "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/kyoto_bg.png" };
	const ofImage face_image{ "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/happy_smiley.png" };
	array<long long int, param::MENU_ITEM_NUM> menu_item_user_id;
	array<int, param::MENU_ITEM_NUM> menu_item_life;
	array<vector<Pin>, param::MENU_ITEM_NUM> pins;
	unordered_map<long long int, SubWindow> sub_windows;
	unordered_map<long long int, ofRectangle> rects_tmp, old_rects, best_rects;

	void transform(unordered_map<long long int, ofRectangle>& old_rects, unordered_map<long long int, ofRectangle>& new_rects);
	void make_optimize_thread();
	void optimize();
	void draw_cursor();
	bool is_intersect_window_pointer();
	bool is_intersect_window_window();
public:
	void setup(HandCursor* const hc);
	void update();
	void draw();

	void add_pin(pair<param::MENU_ITEM_ID, long long int>& id);
	void make_sub_window(pair<param::CONTENT_ID, long long int>& id);

	~SceneManager();
};

#endif