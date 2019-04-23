#pragma once
#ifndef ___Class_SceneManager
#define ___Class_SceneManager

#include "MenuBar.h"
#include "HandCursor.h"
#include "Pin.h"
#include "SubWindow.h"

class SceneManager {
private:
	static constexpr double transform_threshold{ 8000 };
	static constexpr double face_rate{ 2.5 };
	static constexpr double cursor_size{ 250 };
	static const ofColor default_face_color;

	HandCursor* hc;
	MenuBar mb;

	const ofImage map_image{ "./fig/kyoto_bg.png" };
	const ofImage right_hand_image{ "./fig/right_hand.png" };
	const ofImage left_hand_image{ "./fig/left_hand.png" };
	const ofImage face_image{ "./fig/face_tmp.png" };
	array<vector<Pin>, param::MENU_ITEM_NUM> pins;
	unordered_map<long long int, SubWindow> sub_windows;
	bool make_sub_window_flag{ false };

	void draw_face() const;
	void draw_cursor() const;
public:
	array<int, param::MENU_ITEM_NUM> pin_selected_count;

	void setup(HandCursor* const hc);
	void update();
	void draw();

	void make_sub_window(pair<param::CONTENT_ID, long long int>& id);
	void switch_menu_item(int n);

	~SceneManager();
};

#endif