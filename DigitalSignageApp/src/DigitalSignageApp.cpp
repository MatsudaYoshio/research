#include "DigitalSignageApp.h"

void DigitalSignageApp::setup() {
	//ofHideCursor(); // カーソル非表示
	ofEnableAlphaBlending(); // アルファチャンネルを使用可能にする

	//this->sm.setup(&this->hc);
	this->optf.setup(1920, 1080, 24, 9);
}

void DigitalSignageApp::update() {
	//this->hc.update(); // 手カーソルの更新
	//this->sm.update();
	this->optf.update(ofPoint(mouseX, mouseY));
}

void DigitalSignageApp::draw() {
	//ofBackground(255);
	
	//this->sm.draw();
	this->optf.draw();
}

void DigitalSignageApp::exit() {
	//this->hc.exit();
}