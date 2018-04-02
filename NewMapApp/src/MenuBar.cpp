#include "MenuBar.h"

using namespace param;

void MenuBar::setup() {
	items[0].setup(ofColor::gold, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/sightseeing_icon2.png", ofRectangle(DISPLAY_W / 9, DISPLAY_H - DISPLAY_W / 9 - 50, DISPLAY_W / 9, DISPLAY_W / 9), "Sightseeing", 520, 2075);
	items[1].setup(ofColor::lawnGreen, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/restaurant_icon.png", ofRectangle(DISPLAY_W / 3, DISPLAY_H - DISPLAY_W / 9 - 50, DISPLAY_W / 9, DISPLAY_W / 9), "Restaurant", 1375, 2075);
	items[2].setup(ofColor::pink, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/shopping_icon.png", ofRectangle(5 * DISPLAY_W / 9, DISPLAY_H - DISPLAY_W / 9 - 50, DISPLAY_W / 9, DISPLAY_W / 9), "Shopping", 2250, 2075);
	items[3].setup(ofColor::skyBlue, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/hotel_icon.png", ofRectangle(7 * DISPLAY_W / 9, DISPLAY_H - DISPLAY_W / 9 - 50, DISPLAY_W / 9, DISPLAY_W / 9), "Hotel", 3150, 2075);
}

void MenuBar::update() {

}

void MenuBar::draw() {
	ofFill();
	ofSetColor(ofColor::white, 130);
	ofDrawRectangle(0, DISPLAY_H - DISPLAY_W / 9 - 100, DISPLAY_W, DISPLAY_W / 9 + 100);

	for (int i = 0; i < this->item_num; ++i) {
		this->items[i].draw();
	}
}