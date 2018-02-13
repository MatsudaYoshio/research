#include "HandCursorApp.h"
#include "AppParameters.h"

using namespace param;

void HandCursorApp::setup() {}

void HandCursorApp::update() {
	this->hc.update(); // 手カーソルの更新
}

void HandCursorApp::draw() {
	//try {
	//	for (auto& ud : this->hc.user_data) {
	//		ofNoFill();
	//		ofSetLineWidth(60);
	//		ofSetColor(ofColor::white);
	//		ofDrawCircle(ud.second.transformed_cursor_point.x(), ud.second.transformed_cursor_point.y(), 60);
	//		ofFill();
	//		ofSetColor(ud.second.cursor_color);
	//		ofDrawCircle(ud.second.transformed_cursor_point.x(), ud.second.transformed_cursor_point.y(), 55);
	//	}
	//}
	//catch (std::out_of_range&) {}
}

void HandCursorApp::exit() {
	this->hc.exit();
}