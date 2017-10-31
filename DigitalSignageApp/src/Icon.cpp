#include <opencv2/opencv.hpp>

#include "Icon.h"

using namespace cv;

Icon::Icon(const int &x, const int &y, const int &width, const int &height, const string &img_path) {
	this->setup(x, y, width, height, img_path);
}

void Icon::setup(const int &x, const int &y, const int &width, const int &height, const string &img_path) {
	this->rect.set(x, y, width, height);
	this->default_w = width;
	this->default_h = height;

	this->alpha = 255;

	this->state = static_cast<int>(STATE::INACTIVE);

	this->angle = 0.0;

	ofLoadImage(this->texture, img_path);

	this->pb.setup(this->rect.getCenter(), 1.1*(width>>1), 36, 10, ofColor::green);
}

void Icon::update() {
	if (this->pb.is_max_state()) {
		int user_id = this->selected_user_id;
		ofNotifyEvent(this->transition_event, user_id);
		this->state = static_cast<int>(STATE::INACTIVE);
	}

	switch (this->state) {
	case static_cast<int>(STATE::POINT) :
		this->pb.forward_state(2);
		this->alpha = 100;
		break;
	case static_cast<int>(STATE::INACTIVE) :
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