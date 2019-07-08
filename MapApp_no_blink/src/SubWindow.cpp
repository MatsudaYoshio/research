#include "SubWindow.h"

using namespace param;

SubWindow::SubWindow() {}

SubWindow::SubWindow(CONTENT_ID content_id, const long long int user_id, const ofColor& user_color) {
	this->content_id = content_id;
	this->user_id = user_id;

	this->df.setup(user_color, this->frame_size);

	this->view_rect.set(CONTENT_DATA[static_cast<int>(content_id)].x - this->defalut_width / 2, CONTENT_DATA[static_cast<int>(content_id)].y - this->defalut_height / 2, this->defalut_width, this->defalut_height);

	this->font.loadFont("./font/meiryob.ttc", this->number_font_size);
}

void SubWindow::update() {
	if (!this->next_rects.empty()) {
		this->view_rect.set(this->next_rects.front());
		this->next_rects.pop();
	}

	this->df.update(this->view_rect);
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

void SubWindow::set_rect(ofRectangle rect) {
	const ofPoint position_change_value(this->change_rate*(rect.getX() - this->view_rect.getX()), this->change_rate*(rect.getY() - this->view_rect.getY()));
	const auto width_change_value{ this->change_rate*(rect.getWidth() - this->view_rect.getWidth()) };
	const auto height_change_value{ this->change_rate*(rect.getHeight() - this->view_rect.getHeight()) };

	queue<ofRectangle>().swap(this->next_rects);
	rect = move(this->view_rect);
	for (int i = 0; i < this->track_rects_num; ++i) {
		rect.position += position_change_value;
		rect.width += width_change_value;
		rect.height += height_change_value;

		this->next_rects.emplace(rect);
	}
}

long long int SubWindow::get_user_id() const {
	return this->user_id;
}