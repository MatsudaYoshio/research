#include "DigitalSignageApp.h"
#include "MainScene.h"
#include "DetailScene.h"

void DigitalSignageApp::setup() {
	ofSetFrameRate(60);
	// ofHideCursor(); // カーソル非表示
	ofEnableAlphaBlending(); // アルファチャンネルを使用可能にする
	this->sm.setup(&this->hp); // シーンの準備
}

void DigitalSignageApp::update() {
	this->hp.update(); // 手ポインタの更新
	this->sm.update(); // シーンの更新
}

void DigitalSignageApp::draw() {
	ofSetColor(255, 255, 255);
	
	this->sm.draw(); // シーンの描画
	
	/* 手ポインタの描画 */
	for (auto t : this->hp.track_data) {
		ofSetColor(t.second.pointer_color);
		ofCircle(t.second.current_pointer.x, t.second.current_pointer.y, 10);
	}

}

void DigitalSignageApp::exit() {
	this->hp.exit();
}