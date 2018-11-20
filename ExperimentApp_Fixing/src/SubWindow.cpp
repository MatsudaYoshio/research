#include "SubWindow.h"

using namespace param;

SubWindow::SubWindow() {}

SubWindow::SubWindow(CONTENT_ID content_id, const long long int user_id, const ofColor& user_color) {
	this->content_id = content_id;
	this->user_id = user_id;

	this->df.setup(user_color, this->frame_size);

	this->view_rect.set(ofClamp(CONTENT_DATA[static_cast<int>(content_id)].x - this->default_width / 2, 0, DISPLAY_W - this->default_width), ofClamp(CONTENT_DATA[static_cast<int>(content_id)].y - this->default_height / 2, 0, DISPLAY_H - this->default_height), this->default_width, this->default_height);
}

void SubWindow::update() {
	this->df.update(this->view_rect);
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