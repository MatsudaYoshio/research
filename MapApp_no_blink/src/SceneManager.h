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
	static constexpr double transform_threshold{ 8000 };
	static constexpr double face_rate{ 2.5 };
	static constexpr double cursor_size{ 250 };
	static const ofColor default_face_color;

	HandCursor* hc;
	MenuBar mb;
	SimulatedAnnealing sa;

	const ofImage map_image{ "./fig/kyoto_bg.png" };
	const ofImage right_hand_image{ "./fig/right_hand.png" };
	const ofImage left_hand_image{ "./fig/left_hand.png" };
	const ofImage face_image{ "./fig/face.png" };
	array<long long int, param::MENU_ITEM_NUM> menu_item_user_id;
	array<int, param::MENU_ITEM_NUM> menu_item_life;
	array<vector<Pin>, param::MENU_ITEM_NUM> pins;
	unordered_map<long long int, SubWindow> sub_windows, previous_sub_windows;
	unordered_map<long long int, ofRectangle> initial_rects, best_rects, previous_rects;
	double comparative_cost{ 0.0 }, current_cost{ 0.0 }, best_cost{ 0.0 };
	bool make_sub_window_flag{ false };

	void optimize();
	void draw_face() const;
	void draw_cursor() const;
	void release_menu_item(int menu_item_id);
public:
	void setup(HandCursor* const hc);
	void update();
	void draw();

	void update_pin(pair<param::MENU_ITEM_ID, long long int>& id);
	void make_sub_window(pair<param::CONTENT_ID, long long int>& id);

	~SceneManager();
};

#endif