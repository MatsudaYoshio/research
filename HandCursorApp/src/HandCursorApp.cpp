#include "HandCursorApp.h"

void HandCursorApp::setup() {}

void HandCursorApp::update() {
	this->hc.update(); // 手カーソルの更新
}

void HandCursorApp::draw() {}

void HandCursorApp::exit() {
	this->hc.exit();
}