#include <opencv2/opencv.hpp>

#include "Icon.h"

using namespace cv;

Icon::Icon(const int &x, const int &y, const int &width, const int &height, const string &img_path) : alpha(255), state("None") {
	this->setup(x, y, width, height, img_path);
}

void Icon::setup(const int &x, const int &y, const int &width, const int &height, const string &img_path) {
	this->rect.set(x, y, width, height);

	ofLoadImage(this->texture, img_path);

	this->pb.setup(this->rect.getCenter(), 1.1*width/2, 36, 10, ofColor::green);
}

void Icon::update() {
	if (this->state == "point") {
		this->pb.forward_state(2);
		this->alpha = 100;
	}
	else if (this->state == "fadeout") {
		this->angle += this->rotate_speed;
		this->rect.setWidth(max((int)this->rect.width - this->resize_speed, 0));
		this->rect.setHeight(max((int)this->rect.height - this->resize_speed, 0));
		if (this->rect.width == 0 && this->rect.height == 0) {
			this->change_state("fadein");
		}
	}
	else if (this->state == "fadein") {
		this->angle += this->rotate_speed;
		this->rect.width = min((int)this->rect.width + this->resize_speed, 200);
		this->rect.height = min((int)this->rect.height + this->resize_speed, 200);
		if (this->rect.width == 200 && this->rect.height == 200) {
			this->change_state("None");
		}
	}
	else if (this->state == "None") {
		this->pb.reset_state();
		this->alpha = 255;
		if (ofRandom(100) < 1) {
			this->change_state("fadeout");
		}
	}

	if (this->pb.is_max_state()) {
		int pointer_id = this->selected_user_id;
		ofNotifyEvent(this->transition_event, pointer_id);
		this->state = "None";
	}
}

void Icon::draw() {
	if (this->state == "fadeout" || this->state == "fadein") {
		ofPushMatrix();
		ofTranslate(this->rect.getCenter());
		ofRotateZ(this->angle);

		ofSetColor(ofColor::white, this->alpha);
		this->texture.draw(ofPoint(-this->rect.width / 2, -this->rect.height / 2), ofPoint(this->rect.width / 2, -this->rect.height / 2), ofPoint(this->rect.width / 2, this->rect.height / 2), ofPoint(-this->rect.width / 2, this->rect.height / 2));

		ofPopMatrix();
	}
	else if (this->state == "None") {
		ofSetColor(ofColor::white, this->alpha);
		this->texture.draw(this->rect);
	}

	//ofPushMatrix();
	//ofTranslate(this->rect.getCenter());
	//ofRotateZ(angle);

	//ofSetColor(ofColor::white, this->alpha);
	////this->texture.draw(this->tl, this->tr, this->br, this->bl);
	//this->texture.draw(ofPoint(-this->rect.width / 2 + angle/2, -this->rect.height / 2 + angle/2), ofPoint(this->rect.width / 2 - angle/2, -this->rect.height / 2 + angle/2), ofPoint(this->rect.width / 2 - angle/2, this->rect.height / 2 - angle/2), ofPoint(-this->rect.width / 2 + angle/2, this->rect.height / 2 - angle/2));

	//ofPopMatrix();

	///*ofSetColor(ofColor::white, this->alpha);
	//this->tr.x -= 1;
	//this->tr.y += 1;
	//this->bl.x += 1;
	//this->bl.y -= 1;
	//this->tl.x += 1;
	//this->tl.y += 1;
	//this->br.x -= 1;
	//this->br.y -= 1;
	//ofSetColor(ofColor::white, this->alpha);
	//this->texture.draw(this->rect);
	//this->texture.draw(this->tl, this->tr, this->br, this->bl);

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