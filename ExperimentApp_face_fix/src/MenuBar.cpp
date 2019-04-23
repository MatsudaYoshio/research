#include "MenuBar.h"

using namespace param;

void MenuBar::setup(HandCursor* const hc) {
	this->hc = hc;

	/* それぞれのメニュー項目の準備 */
	items[static_cast<int>(MENU_ITEM_ID::SIGHTSEEING)].setup("./fig/sightseeing_icon.png", ofRectangle(DISPLAY_W / 9, DISPLAY_H - DISPLAY_W / 9 - 50, DISPLAY_W / 9, DISPLAY_W / 9), "Sightseeing", 520, 2075, MENU_ITEM_ID::SIGHTSEEING);
	items[static_cast<int>(MENU_ITEM_ID::RESTAURANT)].setup("./fig/restaurant_icon.png", ofRectangle(DISPLAY_W / 3, DISPLAY_H - DISPLAY_W / 9 - 50, DISPLAY_W / 9, DISPLAY_W / 9), "Restaurants", 1367, 2075, MENU_ITEM_ID::RESTAURANT);
	items[static_cast<int>(MENU_ITEM_ID::SHOPPING)].setup("./fig/shopping_icon.png", ofRectangle(5 * DISPLAY_W / 9, DISPLAY_H - DISPLAY_W / 9 - 50, DISPLAY_W / 9, DISPLAY_W / 9), "Shopping", 2250, 2075, MENU_ITEM_ID::SHOPPING);
	items[static_cast<int>(MENU_ITEM_ID::HOTEL)].setup("./fig/hotel_icon.png", ofRectangle(7 * DISPLAY_W / 9, DISPLAY_H - DISPLAY_W / 9 - 50, DISPLAY_W / 9, DISPLAY_W / 9), "Hotels", 3142, 2075, MENU_ITEM_ID::HOTEL);
}

void MenuBar::update() {
	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		this->items[i].update();
	}
};

void MenuBar::draw() const {
	/* メニュー項目の薄い背景を描画 */
	ofFill();
	ofSetColor(ofColor::white, HALF_MAX_ALFHA);
	ofDrawRectangle(0, DISPLAY_H - DISPLAY_W / 9 - 100, DISPLAY_W, DISPLAY_W / 9 + 100);

	/* メニュー項目の描画 */
	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		this->items[i].draw();
	}
}

void MenuBar::activate_menu_item(const int menu_item_id) {
	this->items[menu_item_id].set_state(MenuItem::STATE::ACTIVE);
}

void MenuBar::deactivate_menu_item(const int menu_item_id) {
	this->items[menu_item_id].set_state(MenuItem::STATE::INACTIVE);
}

bool MenuBar::is_activated(const int menu_item_id) const {
	return this->items[menu_item_id].is_activated();
}