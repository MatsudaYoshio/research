#include "HandCursorApp.h"
#include "AppParameters.h"

using namespace param;

void HandCursorApp::setup() {}

void HandCursorApp::update() {
	this->hc.update(); // 手カーソルの更新
}

void HandCursorApp::draw() {

	//for (const auto& ud : this->hc.user_data) {
	//	ofNoFill();
	//	ofSetLineWidth(60);
	//	ofSetColor(ofColor::white);
	//	ofDrawCircle(DISPLAY_W - RESOLUTION_RATIO_W*ud.second.cursor_point_x, RESOLUTION_RATIO_H*ud.second.cursor_point_y, 60);
	//	ofFill();
	//	ofSetColor(ofColor::black);
	//	ofDrawCircle(DISPLAY_W - RESOLUTION_RATIO_W*ud.second.cursor_point_x, RESOLUTION_RATIO_H*ud.second.cursor_point_y, 55);
	//}

	//const auto numberPeopleDetected = this->hc.pose_key_points.get_read_position().getSize(0);
	//for (int i = 0; i < numberPeopleDetected; ++i) {
	//	ofNoFill();
	//	ofSetLineWidth(60);
	//	ofSetColor(ofColor::white);
	//	ofDrawCircle(DISPLAY_W - RESOLUTION_RATIO_W*(CAMERA_W / 100)*this->hc.pose_key_points.get_read_position()[{i, 4, 0}], RESOLUTION_RATIO_H *(CAMERA_H / 100)*this->hc.pose_key_points.get_read_position()[{i, 4, 1}], 60);
	//	ofFill();
	//	ofSetColor(ofColor::black);
	//	ofDrawCircle(DISPLAY_W - RESOLUTION_RATIO_W*(CAMERA_W / 100)*this->hc.pose_key_points.get_read_position()[{i, 4, 0}], RESOLUTION_RATIO_H *(CAMERA_H / 100)*this->hc.pose_key_points.get_read_position()[{i, 4, 1}], 55);
	//}
}

void HandCursorApp::exit() {
	this->hc.exit();
}