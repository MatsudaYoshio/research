#include "SubWindow.h"

using namespace param;

SubWindow::SubWindow(CONTENT_ID content_id, const long long int user_id) {
	this->content_id = content_id;
	this->user_id = user_id;

	this->window.setup(CONTENT_DATA[static_cast<int>(content_id)].x - (this->defalut_width >> 1), CONTENT_DATA[static_cast<int>(content_id)].y - (this->defalut_height >> 1), this->defalut_width, this->defalut_height, true);
	this->window.show();

	this->df.setup(MENU_ITEM_COLOR[static_cast<int>(CONTENT_DATA[static_cast<int>(content_id)].menu_item_id)], this->frame_size);

	this->view_rect.set(0, 0, this->defalut_width, this->defalut_height);

	this->track_index = this->TRACK_READY;

	this->image.load(CONTENT_DATA[static_cast<int>(content_id)].image_path);
}

void SubWindow::update() {
	if (this->track_index == this->track_rects_num) {
		this->track_index = this->TRACK_READY;
	}
	else if (this->track_index != this->TRACK_READY) {
		this->window.setWindowPosition(this->track_rects[this->track_index].getX(), this->track_rects[this->track_index].getY());
		this->window.setWindowSize(this->track_rects[this->track_index].getWidth(), this->track_rects[this->track_index].getHeight());
		++this->track_index;
	}
		
	this->view_rect.setWidth(this->window.getWidth());
	this->view_rect.setHeight(this->window.getHeight());

	this->font.loadFont("meiryob.ttc", 30 * this->view_rect.getHeight() / this->defalut_height);

	this->df.update(this->view_rect);
}

void SubWindow::draw() {
	this->window.begin(); // サブウィンドウの描画開始

	gluLookAt(this->view_rect.getX(), this->view_rect.getY(), 0, this->view_rect.getX(), this->view_rect.getY(), -1, 0, 1, 0); // 視点移動

	ofBackground(ofColor::white);

	ofSetColor(ofColor::black);
	this->font.drawString(CONTENT_DATA[static_cast<int>(content_id)].name, this->frame_size, this->frame_size + 40 * this->view_rect.getHeight() / this->defalut_height);

	ofSetColor(ofColor::white);
	this->image.draw(this->frame_size, this->frame_size + 0.12*this->view_rect.getHeight(), this->view_rect.getWidth() - (this->frame_size >> 1), this->view_rect.getHeight() - (this->frame_size >> 1));

	//ofBackground(ofColor::white);
	//this->scene->draw(); // シーンの描画

	this->df.draw(); // 枠の描画

	this->window.end(); // サブウィンドウのの描画終了
}

ofRectangle SubWindow::get_rect() const {
	return this->window.get_rect();
}

long long int SubWindow::get_user_id() const {
	return this->user_id;
}

void SubWindow::exit() {
	this->window.hide();
	this->window.close();
}