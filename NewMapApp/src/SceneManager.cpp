#include "SceneManager.h"

using namespace param;

void SceneManager::setup(HandCursor* const hc) {
	this->hc = hc;

	this->mb.setup(hc);
	ofAddListener(this->mb.add_pin_event, this, &SceneManager::add_pin);

	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		this->pins[i].resize(MENU_ITEM_CONTENTS[i].size());
		for (int j = 0; j < this->pins[i].size(); ++j) {
			this->pins[i][j].setup(MENU_ITEM_CONTENTS[i][j]);
		}
	}

	this->ab.setup(&this->menu_item_flag);
}

void SceneManager::update() {
	this->mb.update();
	this->ab.update();
}

void SceneManager::draw() {
	ofSetColor(ofColor::white);
	this->map_image.draw(0, 0, DISPLAY_W, DISPLAY_H); // マップの表示

	this->mb.draw(); // メニューバーの表示

	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		if (this->menu_item_flag[i]) {
			for (const auto& p : this->pins[i]) {
				p.draw();
			}
		}
	}

	this->ab.draw();

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

void SceneManager::add_pin(param::MENU_ITEM_ID& item_id) {
	this->menu_item_flag[static_cast<int>(item_id)] = true;
}

SceneManager::~SceneManager() {
	ofRemoveListener(this->mb.add_pin_event, this, &SceneManager::add_pin);
}