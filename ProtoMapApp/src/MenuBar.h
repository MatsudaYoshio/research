#pragma once
#ifndef ___Class_MenuBar
#define ___Class_MenuBar

#include "MenuItem.h"
#include "HandCursor.h"

class MenuBar {
private:
	HandCursor* hc;
	array<MenuItem, param::MENU_ITEM_NUM> items;
public:
	ofEvent<pair<param::MENU_ITEM_ID, long long int>> add_pin_event;

	void setup(HandCursor* const hc);
	void update();
	void draw() const;

	void deactivate_menu_item(int menu_item_id);
	void select_menu_item(pair<param::MENU_ITEM_ID, long long int>& id);

	~MenuBar();
};

#endif