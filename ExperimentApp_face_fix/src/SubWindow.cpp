#include "SubWindow.h"

using namespace param;

SubWindow::SubWindow() {}

SubWindow::SubWindow(CONTENT_ID content_id, const long long int user_id, const ofColor& user_color, const ofPoint& position) {
	this->content_id = content_id;
	this->user_id = user_id;

	this->view_rect.set(ofClamp(position.x - MAX_SUB_WINDOW_W / 2, 0, DISPLAY_W - MAX_SUB_WINDOW_W), ofClamp(position.y - MAX_SUB_WINDOW_H / 2, 0, DISPLAY_H - MAX_SUB_WINDOW_H), MAX_SUB_WINDOW_W, MAX_SUB_WINDOW_H);
	this->df.setup(user_color, this->view_rect);

	this->font.loadFont("./font/meiryob.ttc", this->number_font_size);
}

void SubWindow::draw() {
	ofSetColor(ofColor::white);
	CONTENT_IMAGE[static_cast<int>(this->content_id)].draw(this->view_rect);

	ofSetColor(MENU_ITEM_COLOR[static_cast<int>(CONTENT_DATA[static_cast<int>(this->content_id)].menu_item_id)]);
	ofDrawCircle(this->view_rect.x + 80, this->view_rect.y + 80, this->number_circle_size);
	ofSetColor(ofColor::black);
	this->font.drawString(to_string(CONTENT_DATA[static_cast<int>(this->content_id)].number), this->view_rect.x + 53, this->view_rect.y + 110);

	this->df.draw(); // ˜g‚Ì•`‰æ
}

ofRectangle SubWindow::get_rect() const {
	return this->view_rect;
}

long long int SubWindow::get_user_id() const {
	return this->user_id;
}