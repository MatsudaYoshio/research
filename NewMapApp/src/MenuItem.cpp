#include "MenuItem.h"

using namespace param;

void MenuItem::setup(const ofColor color, const string& image_path, const ofRectangle& item_rect, const string& item_name, const int font_x, const int font_y) {
	this->item_name = item_name;
	this->color = color;
	ofLoadImage(this->texture, image_path);
	this->item_rect.set(item_rect);
	auto texture_size{ this->item_rect.getWidth()*this->texture_size_ratio };
	this->texture_rect.set(this->item_rect.getCenter() - texture_size / 2, texture_size, texture_size);
	this->font.loadFont("meiryob.ttc", this->font_size);
	this->font_x = font_x;
	this->font_y = font_y;
}

void MenuItem::draw() {
	ofFill();
	ofSetColor(this->color);
	ofDrawRectangle(this->item_rect);
	ofSetColor(ofColor::white);
	this->texture.draw(this->texture_rect);
	ofSetColor(ofColor::black);
	this->font.drawString(this->item_name, this->font_x, this->font_y);
}