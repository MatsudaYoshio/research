#include "SubWindow.h"

using namespace param;

SubWindow::SubWindow() {}

SubWindow::SubWindow(CONTENT_ID content_id, const long long int user_id, const ofColor& user_color) {
	this->content_id = content_id;
	this->user_id = user_id;

	this->view_rect.set(ofClamp(CONTENT_DATA[static_cast<int>(content_id)].x - MAX_SUB_WINDOW_W / 2, 0, DISPLAY_W - MAX_SUB_WINDOW_W), ofClamp(CONTENT_DATA[static_cast<int>(content_id)].y - MAX_SUB_WINDOW_H / 2, 0, DISPLAY_H - MAX_SUB_WINDOW_H), MAX_SUB_WINDOW_W, MAX_SUB_WINDOW_H);
	this->df.setup(user_color, this->view_rect);
}

void SubWindow::draw() {
	ofSetColor(ofColor::white);
	CONTENT_IMAGE[static_cast<int>(this->content_id)].draw(this->view_rect);

	this->df.draw(); // ˜g‚Ì•`‰æ
}

ofRectangle SubWindow::get_rect() const {
	return this->view_rect;
}

long long int SubWindow::get_user_id() const {
	return this->user_id;
}