#include "DigitalSignageApp.h"

void DigitalSignageApp::setup() {
	ofHideCursor(); // カーソル非表示
	ofEnableAlphaBlending(); // アルファチャンネルを使用可能にする

	this->hc.new_thread_update();
	
	this->sm.setup(&this->hc);
}

void DigitalSignageApp::update() {
	this->sm.update();
}

void DigitalSignageApp::draw() {
	this->sm.draw();
}

void DigitalSignageApp::exit() {
	this->hc.exit();
}