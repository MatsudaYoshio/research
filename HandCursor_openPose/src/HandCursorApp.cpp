#include "HandCursorApp.h"
#include "AppParameters.h"

using namespace param;

void HandCursorApp::setup() {}

void HandCursorApp::update() {
	this->hc.update(); // 手カーソルの更新
}

void HandCursorApp::draw() {
	const auto numberPeopleDetected = this->hc.pose_key_points.getSize(0);
	for (int i = 0; i < numberPeopleDetected; ++i) {
		ofNoFill();
		ofSetLineWidth(60);
		ofSetColor(ofColor::white);
		ofDrawCircle(DISPLAY_W - RESOLUTION_RATIO_W*this->hc.pose_key_points[{i, 4, 0}], RESOLUTION_RATIO_H *this->hc.pose_key_points[{i, 4, 1}], 60);
		ofFill();
		ofSetColor(ofColor::black);
		ofDrawCircle(DISPLAY_W - RESOLUTION_RATIO_W*this->hc.pose_key_points[{i, 4, 0}], RESOLUTION_RATIO_H *this->hc.pose_key_points[{i, 4, 1}], 55);
	}
}

void HandCursorApp::exit() {
	this->hc.exit();
}