#include "DigitalSignageApp.h"

void DigitalSignageApp::setup() {
	//ofHideCursor(); // カーソル非表示
	ofEnableAlphaBlending(); // アルファチャンネルを使用可能にする

	/* メインシーンの準備 */
	ofAddListener(this->ms.point_event, this, &DigitalSignageApp::pointed);
	ofAddListener(this->ms.make_sub_window_event, this, &DigitalSignageApp::make_sub_window);
	this->ms.setup(&this->hc);

	this->ga.setup(&this->hc); // 遺伝的アルゴリズムの準備
}

void DigitalSignageApp::update() {
	this->hc.update(); // 手カーソルの更新

	this->ms.update(); // メインシーンの更新

	/* 消滅したカーソルがあれば選択しているユーザリストから消す */
	for (auto id = begin(this->selected_users); id != end(this->selected_users);) {
		if (this->hc.track_data.find(*id) == end(this->hc.track_data)) {
			this->selected_users.erase(id++);
		}
		else {
			++id;
		}
	}
	this->ga(this->selected_users, this->ms.user_id_list);
}

void DigitalSignageApp::draw() {

	this->ms.draw(); // メインシーンの描画

	if (this->selected_users.size() > 0) {
		this->ga.draw();
	}

	///* 手カーソルの描画 */
	//for (const auto& td : this->hc.track_data) {
	//	int alpha = 255;
	//	double r = 1;
	//	for (int i = 0; i < 100; ++i) {
	//		r += 3;
	//		alpha -= 12;
	//		ofSetColor(td.second.cursor_color, alpha);
	//		ofCircle(param::W - td.second.current_pointer.x, td.second.current_pointer.y, r);
	//	}
	//}
}

void DigitalSignageApp::exit() {
	this->hc.exit();
}

void DigitalSignageApp::pointed(pair<int, int> &id) {
	this->ms.point_icon(id.first, id.second);
}

void DigitalSignageApp::make_sub_window(pair<int, int>& id) {
	this->selected_users.emplace(id.second);
}