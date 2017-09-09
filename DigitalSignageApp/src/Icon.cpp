#include <opencv2/opencv.hpp>

#include "Icon.h"

using namespace cv;

Icon::Icon(const int x, const int y, const int width, const int height, const string img_path) {
	this->setup(x, y, width, height, img_path);
}

void Icon::setup(const int x, const int y, const int width, const int height, const string img_path) {
	this->rect.set(x, y, width, height);
	this->tl.set(x + width * this->frame_ratio / 2, y + height * this->frame_ratio / 2);
	this->tr.set(x + width * (1 - this->frame_ratio / 2), y + height * this->frame_ratio / 2);
	this->bl.set(x + width * this->frame_ratio / 2, y + height * (1 - this->frame_ratio / 2));
	this->br.set(x + width * (1 - this->frame_ratio / 2), y + height * (1 - this->frame_ratio / 2));
	ofLoadImage(this->texture, img_path);
}

void Icon::update() {

}

void Icon::draw() {
	ofSetColor(255, 255, 255);
	ofNoFill();
	ofSetLineWidth(10);
	ofRect(this->rect);
	ofSetColor(ofColor::white);
	this->texture.draw(this->tl, this->tr, this->br, this->bl);
}

bool Icon::is_inside(const ofPoint &p) const {
	return this->rect.inside(p);
}