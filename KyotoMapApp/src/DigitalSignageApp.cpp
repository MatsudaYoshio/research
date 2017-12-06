#include "DigitalSignageApp.h"

void DigitalSignageApp::setup() {
	ofHideCursor(); // カーソル非表示
	ofEnableAlphaBlending(); // アルファチャンネルを使用可能にする

	this->sm.setup(&this->hc);
}

void DigitalSignageApp::update() {
	this->hc.update(); // 手カーソルの更新

	this->sm.update();
}

void DigitalSignageApp::draw() {
	this->sm.draw();
}

void DigitalSignageApp::exit() {
	this->hc.exit();
}