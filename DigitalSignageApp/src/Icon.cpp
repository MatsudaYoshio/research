#include <opencv2/opencv.hpp>

#include "Icon.h"

using namespace cv;

Icon::Icon(const int &x, const int &y, const int &width, const int &height, const string &img_path) :frame_r(255), frame_g(255), frame_b(255), alpha(255), state("None") {
	this->setup(x, y, width, height, img_path);
}

void Icon::setup(const int &x, const int &y, const int &width, const int &height, const string &img_path) {
	this->rect.set(x, y, width, height);
	
	this->tl.set(x + width * this->frame_ratio / 2, y + height * this->frame_ratio / 2);
	this->tr.set(x + width * (1 - this->frame_ratio / 2), y + height * this->frame_ratio / 2);
	this->bl.set(x + width * this->frame_ratio / 2, y + height * (1 - this->frame_ratio / 2));
	this->br.set(x + width * (1 - this->frame_ratio / 2), y + height * (1 - this->frame_ratio / 2));
	
	ofLoadImage(this->texture, img_path);

	this->pb.setup(this->rect.getCenter(), 1.1*width/2, 36, 10, ofColor::green);
}

void Icon::update() {
	if (this->state == "point") {
		this->pb.forward_state(2);
		this->alpha = 100;
	}
	else if (this->state == "None") {
		this->pb.reset_state();
		this->frame_r = this->frame_g = this->frame_b = this->alpha = 255;
	}

	if (this->pb.is_max_state()) {
		int pointer_id = this->selected_user_id;
		ofNotifyEvent(this->transition_event, pointer_id);
		this->state = "None";
		this->frame_r = this->frame_g = this->frame_b = this->alpha = 255;
	}
}

void Icon::draw() {
	ofSetColor(ofColor::white, this->alpha);
	this->texture.draw(this->tl, this->tr, this->br, this->bl);
	ofFill();

	if (this->state == "point") {
		this->pb.draw();
	}
}

bool Icon::is_inside(const ofPoint &p) const {
	return this->rect.inside(p);
}

void Icon::change_state(const string &state) {
	this->state = state;
}

void Icon::set_user_id(const int &user_id) {
	this->selected_user_id = user_id;
}