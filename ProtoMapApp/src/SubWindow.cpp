#include "SubWindow.h"

using namespace param;

SubWindow::SubWindow(CONTENT_ID content_id, const long long int user_id) {
	this->content_id = content_id;
	this->user_id = user_id;

	this->df.setup(MENU_ITEM_COLOR[static_cast<int>(CONTENT_DATA[static_cast<int>(content_id)].menu_item_id)], this->frame_size);

	this->view_rect.set(CONTENT_DATA[static_cast<int>(content_id)].x - this->defalut_width / 2, CONTENT_DATA[static_cast<int>(content_id)].y - this->defalut_height / 2, this->defalut_width, this->defalut_height);

	this->track_index = this->TRACK_READY;
}

void SubWindow::update() {
	if (this->track_index == this->track_rects_num) {
		this->track_index = this->TRACK_READY;
	}
	else if (this->track_index != this->TRACK_READY) {
		this->view_rect = this->track_rects[this->track_index++];
	}

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