#include "SceneManager.h"

using namespace param;

void SceneManager::setup(HandCursor* const hc) {
	this->hc = hc;
	this->mb.setup(hc);
}

void SceneManager::update() {
	this->mb.update();
}

void SceneManager::draw() {
	ofSetColor(ofColor::white);
	this->map_image.draw(0, 0, DISPLAY_W, DISPLAY_H); // マップの表示
	this->mb.draw(); // メニューバーの表示

	/* 手カーソルの描画 */
	for (const auto& ud : this->hc->user_data) {
		ofNoFill();
		ofSetLineWidth(60);
		ofSetColor(ofColor::white);
		ofDrawCircle(ud.second.transformed_cursor_point.x(), ud.second.transformed_cursor_point.y(), 60);
		ofFill();
		ofSetColor(ud.second.cursor_color);
		ofDrawCircle(ud.second.transformed_cursor_point.x(), ud.second.transformed_cursor_point.y(), 55);
	}
}