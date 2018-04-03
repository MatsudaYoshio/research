#include "MapApp.h"

using namespace param;

void MapApp::setup() {
	ofHideCursor(); // カーソル非表示
	ofEnableAlphaBlending(); // アルファチャンネルを使用可能にする
	
	this->sm.setup(&this->hc);
}

void MapApp::update() {
	this->hc.update();
	this->sm.update();
}

void MapApp::draw() {
	this->sm.draw();
}

void MapApp::exit() {
	this->hc.exit();
}