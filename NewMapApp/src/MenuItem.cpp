#include "MenuItem.h"

using namespace param;

void MenuItem::setup(const ofColor color, const char* image_path, const ofRectangle& item_rect) {
	this->color = color;
	ofLoadImage(this->texture, image_path);
	this->item_rect.set(item_rect);
	auto texture_size{ this->item_rect.getWidth()*this->texture_size_ratio };
	this->texture_rect.set(this->item_rect.getCenter() - texture_size / 2, texture_size, texture_size);
}

void MenuItem::update() {

}

void MenuItem::draw() {
	ofFill();
	ofSetColor(this->color);
	ofDrawRectangle(this->item_rect);
	ofSetColor(ofColor::white);
	this->texture.draw(this->texture_rect);
}