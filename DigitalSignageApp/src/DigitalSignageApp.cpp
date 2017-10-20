#include "DigitalSignageApp.h"

void DigitalSignageApp::setup() {
	//ofHideCursor(); // カーソル非表示
	ofEnableAlphaBlending(); // アルファチャンネルを使用可能にする

	this->sm.setup(&this->hc);
	//this->optf.setup(1920, 1080, 24, 9);
}

void DigitalSignageApp::update() {
	this->hc.update(); // 手カーソルの更新
	this->sm.update();
	//this->optf.update(ofPoint(mouseX, mouseY));
}

void DigitalSignageApp::draw() {
	//ofBackground(255);
	//int test[101] = {};
	//test[50] = -50;
	//for (int i = 0; i < 50; ++i) {
	//	test[i] = test[100 - i] = -1 * i;
	//}
	//
	////int test[] = { -5, -10, -15, -20, -25, -30, -35, -40, -45, -50, -45, -40, -30, -20, -15, -10, -5 };
	//ofBackgroundGradient(ofColor::aquamarine+test[i], ofColor::blueSteel + test[i], OF_GRADIENT_CIRCULAR);
	//i %= 100;
	//i++;
	this->sm.draw();
	ofSetColor(ofColor::black);
	//ofDrawBitmapString(ofToString((int)ofGetFrameRate()), 100, 100);
	//this->optf.draw();
}

void DigitalSignageApp::exit() {
	this->hc.exit();
}