#include <opencv2/opencv.hpp>

#include "Icon.h"

using namespace cv;

Icon::Icon(const int x, const int y, const int width, const int height, const string img_path) {
	this->setup(x, y, width, height, img_path);
}

void Icon::setup(const int x, const int y, const int width, const int height, const string img_path) {
	this->rect.set(x, y, width, height);
	this->tl.set(x, y);
	this->tr.set(x + width, y);
	this->bl.set(x, y + height);
	this->br.set(x + width, y + height);
	ofLoadImage(this->texture, img_path);
}

void Icon::update() {

}

void Icon::draw() {
	this->texture.draw(this->tl, this->tr, this->br, this->bl);
}

bool Icon::is_inside(const ofPoint &p) const {
	return this->rect.inside(p);
}