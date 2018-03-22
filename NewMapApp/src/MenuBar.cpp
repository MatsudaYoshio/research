#include "MenuBar.h"

using namespace param;

void MenuBar::setup() {
	//icons[0].set(DISPLAY_W / 9, DISPLAY_H - DISPLAY_W / 9, DISPLAY_W / 9, DISPLAY_W / 9);
	//icons[1].set(DISPLAY_W / 3, DISPLAY_H - DISPLAY_W / 9, DISPLAY_W / 9, DISPLAY_W / 9);
	//icons[2].set(5 * DISPLAY_W / 9, DISPLAY_H - DISPLAY_W / 9, DISPLAY_W / 9, DISPLAY_W / 9);
	//icons[3].set(7 * DISPLAY_W / 9, DISPLAY_H - DISPLAY_W / 9, DISPLAY_W / 9, DISPLAY_W / 9);

	icons[0].set(0, DISPLAY_H - DISPLAY_W / 9, DISPLAY_W / 4, DISPLAY_W / 9);
	icons[1].set(DISPLAY_W / 4, DISPLAY_H - DISPLAY_W / 9, DISPLAY_W / 4, DISPLAY_W / 9);
	icons[2].set(DISPLAY_W / 2, DISPLAY_H - DISPLAY_W / 9, DISPLAY_W / 4, DISPLAY_W / 9);
	icons[3].set(3 * DISPLAY_W / 4, DISPLAY_H - DISPLAY_W / 9, DISPLAY_W / 4, DISPLAY_W / 9);
}

void MenuBar::update() {

}

void MenuBar::draw() {
	ofFill();
	ofDrawRectangle(0, DISPLAY_H - DISPLAY_W / 9, DISPLAY_W, DISPLAY_W / 9);
	ofSetColor(ofColor::orange);
	ofDrawRectangle(this->icons[0]);
	ofSetColor(ofColor::pink);
	ofDrawRectangle(this->icons[1]);
	ofSetColor(ofColor::blue);
	ofDrawRectangle(this->icons[2]);
	ofSetColor(ofColor::purple);
	ofDrawRectangle(this->icons[3]);
}