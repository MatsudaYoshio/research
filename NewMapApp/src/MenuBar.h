#pragma once
#ifndef ___Class_MenuBar
#define ___Class_MenuBar

#include "AppParameters.h"
#include "MenuItem.h"
#include "HandCursor.h"

class MenuBar {
private:
	HandCursor* hc;
	array<MenuItem, param::MENU_ITEM_NUM> items;
public:
	ofEvent<param::MENU_ITEM_ID> add_pin_event;

	void setup(HandCursor* const hc);
	void update();
	void draw() const;

	void select_menu_item(param::MENU_ITEM_ID& item_id);

	~MenuBar();
};

#endif