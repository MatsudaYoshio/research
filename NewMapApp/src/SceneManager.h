#pragma once
#ifndef ___Class_SceneManager
#define ___Class_SceneManager

#include "AppParameters.h"
#include "MenuBar.h"
#include "HandCursor.h"
#include "Pin.h"

class SceneManager {
private:
	HandCursor* hc;
	MenuBar mb;
	const ofImage map_image{ "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/kyoto_bg.png" };
	array<bool, param::MENU_ITEM_NUM> pin_flag;
	array<vector<Pin>, param::MENU_ITEM_NUM> pins;
	//array<vector<Pin>, param::MENU_ITEM_NUM> pins{
	//	vector<Pin>{Pin{1,param::CONTENT_ID::KYOTO_TOWER, 100, 300}, Pin{ 2,param::CONTENT_ID::HIGASHIHONGANJI, 500, 500 }, Pin{ 3,param::CONTENT_ID::SYOSEIEN, 1200, 400 }, Pin{4, param::CONTENT_ID::NISHIHONGANJI, 1500, 1000 }, Pin{5, param::CONTENT_ID::RYUKOKU_MUSEUM, 1290, 900 }, Pin{6, param::CONTENT_ID::KYOTO_AQUARIUM, 400, 1600 }}
	//};
public:
	void setup(HandCursor* const hc);
	void update();
	void draw() const;

	void add_pin(param::MENU_ITEM_ID& item_id);

	~SceneManager();
};

#endif