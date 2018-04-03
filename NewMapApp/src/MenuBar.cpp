#include "MenuBar.h"

using namespace param;

void MenuBar::setup(HandCursor* const hc) {
	this->hc = hc;

	items[static_cast<int>(MENU_ITEM_ID::SIGHTSEEING)].setup(ofColor::gold, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/sightseeing_icon2.png", ofRectangle(DISPLAY_W / 9, DISPLAY_H - DISPLAY_W / 9 - 50, DISPLAY_W / 9, DISPLAY_W / 9), "Sightseeing", 520, 2075, MENU_ITEM_ID::SIGHTSEEING);
	items[static_cast<int>(MENU_ITEM_ID::RESTAURANT)].setup(ofColor::lawnGreen, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/restaurant_icon.png", ofRectangle(DISPLAY_W / 3, DISPLAY_H - DISPLAY_W / 9 - 50, DISPLAY_W / 9, DISPLAY_W / 9), "Restaurant", 1375, 2075, MENU_ITEM_ID::RESTAURANT);
	items[static_cast<int>(MENU_ITEM_ID::SHOPPING)].setup(ofColor::pink, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/shopping_icon.png", ofRectangle(5 * DISPLAY_W / 9, DISPLAY_H - DISPLAY_W / 9 - 50, DISPLAY_W / 9, DISPLAY_W / 9), "Shopping", 2250, 2075, MENU_ITEM_ID::SHOPPING);
	items[static_cast<int>(MENU_ITEM_ID::HOTEL)].setup(ofColor::skyBlue, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/hotel_icon.png", ofRectangle(7 * DISPLAY_W / 9, DISPLAY_H - DISPLAY_W / 9 - 50, DISPLAY_W / 9, DISPLAY_W / 9), "Hotel", 3150, 2075, MENU_ITEM_ID::HOTEL);
}

void MenuBar::update() {
	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		for (const auto& ud : this->hc->user_data) {
			if (this->items[i].is_inside(ud.second.transformed_cursor_point.x(), ud.second.transformed_cursor_point.y())) {
				this->items[i].set_state(MenuItem::STATE::POINT);
				goto POINTED;
			}
		}

		this->items[i].set_state(MenuItem::STATE::INACTIVE);
	POINTED:;
		this->items[i].update();
	}
}

void MenuBar::draw() {
	ofFill();
	ofSetColor(ofColor::white, 130);
	ofDrawRectangle(0, DISPLAY_H - DISPLAY_W / 9 - 100, DISPLAY_W, DISPLAY_W / 9 + 100);

	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		this->items[i].draw();
	}
}