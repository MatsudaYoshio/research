#include <opencv2/opencv.hpp>

#include "Icon.h"

using namespace cv;

void Icon::fireEvent() {
	float val = 10;
	ofNotifyEvent(this->my_event, val);
}

void Icon::setup(const int x, const int y, const int width, const int height, const string img_path) {
	this->state = false;
	this->rect.set(x, y, width, height);
	this->tl.set(x, y);
	this->tr.set(x + width, y);
	this->bl.set(x, y + height);
	this->br.set(x + width, y + height);
	ofLoadImage(this->texture, img_path);
}

void Icon::draw() {
	if (this->state) {
		this->tl.set(this->tl.x + 1, this->tl.y);
	}
	this->texture.draw(this->tl, this->tr, this->br, this->bl);
}

void Icon::update_state(const ofPoint &p) {
	this->state = this->rect.inside(p);
}