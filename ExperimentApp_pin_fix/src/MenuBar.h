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
	void setup(HandCursor* const hc);
	void update();
	void draw() const;

	void activate_menu_item(int menu_item_id);
	void deactivate_menu_item(int menu_item_id);
	bool is_activated(int menu_item_id) const;
};

#endif