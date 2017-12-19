#include "Icon.h"

void Icon::setup(const int &x, const int &y, const int &width, const int &height, const string &img_path, const int &content_id) {
	this->content_id = content_id;
	this->rect.set(x, y, width, height);
	this->default_rect.set(this->rect);
	this->big_rect.set(this->default_rect.getX() - this->default_rect.getWidth()*(this->expansion_ratio - 1) / 2, this->default_rect.getY() - this->default_rect.getHeight()*(this->expansion_ratio - 1) / 2, this->default_rect.getWidth()*this->expansion_ratio, this->default_rect.getHeight()*this->expansion_ratio);

	this->alpha = 255;

	this->state = static_cast<int>(STATE::INACTIVE);

	this->angle = 0.0;

	ofLoadImage(this->texture, img_path);

	this->pb.setup(this->big_rect.getCenter(), 1.1*this->expansion_ratio*width / 2, 48, 14, ofColor::green);
}

void Icon::update() {
	if (this->pb.is_max_state()) {
		pair<int, int> id(this->content_id, this->selected_user_id); // コンテンツidと選択したユーザid
		ofNotifyEvent(this->select_event, id);
		this->state = static_cast<int>(STATE::INACTIVE);
	}

	switch (this->state) {
	case static_cast<int>(STATE::POINT) :
		this->rect.set(this->big_rect);
		this->pb.forward_state(1);
		this->alpha = 100;
		break;
	case static_cast<int>(STATE::INACTIVE) :
		this->rect.set(this->default_rect);
		this->pb.reset_state();
		this->alpha = 255;
		break;
	}
}

void Icon::draw() {
	switch (this->state) {
	case static_cast<int>(STATE::POINT) :
		this->pb.draw();
	case static_cast<int>(STATE::INACTIVE) :
		ofSetColor(ofColor::white, this->alpha);
		this->texture.draw(this->rect);
		break;
	}
}

bool Icon::is_inside(const ofPoint &p) const {
	return this->rect.inside(p);
}

void Icon::change_state(const STATE& state) {
	this->state = static_cast<int>(state);
}

void Icon::set_user_id(const int &user_id) {
	this->selected_user_id = user_id;
}

int Icon::get_content_id() const {
	return this->content_id;
}