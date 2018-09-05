#include "WhacAMoleApp.h"

const ofColor WhacAMoleApp::bg_color{ 33, 215, 102 };

void WhacAMoleApp::setup() {
	ofSetBackgroundAuto(false); // フレーム更新時に塗りつぶしを無効化
	ofHideCursor(); // カーソル非表示
}

void WhacAMoleApp::update() {
}

void WhacAMoleApp::draw() {
	ofBackground(this->bg_color); // 背景の描画

	this->hammer_image.draw(mouseX, mouseY);
}