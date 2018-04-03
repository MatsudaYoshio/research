#include "MenuItem.h"

using namespace param;

void MenuItem::setup(const ofColor& color, const string& image_path, const ofRectangle& item_rect, const string& item_name, const int font_x, const int font_y, const MENU_ITEM_ID item_id) {
	this->item_name = item_name;
	this->item_id = item_id;
	this->color = color;
	ofLoadImage(this->texture, image_path);
	this->item_rect.set(item_rect);
	auto texture_size{ this->item_rect.getWidth()*this->texture_size_ratio };
	this->texture_rect.set(this->item_rect.getCenter() - texture_size / 2, texture_size, texture_size);
	this->font.loadFont("meiryob.ttc", this->font_size);
	this->font_x = font_x;
	this->font_y = font_y;
}

void MenuItem::update() {
	switch (this->state) {
	case STATE::POINT:
		this->alpha = 100;
		break;
	case STATE::INACTIVE:
		this->alpha = ofColor::limit();
		break;
	}
}

void MenuItem::draw() const {
	ofFill();
	ofSetColor(this->color, this->alpha);
	ofDrawRectangle(this->item_rect);
	ofSetColor(ofColor::white, this->alpha);
	this->texture.draw(this->texture_rect);
	ofSetColor(ofColor::black, this->alpha);
	this->font.drawString(this->item_name, this->font_x, this->font_y);
}

bool MenuItem::is_inside(const int x, const int y) const {
	return this->item_rect.inside(x, y);
}

void MenuItem::set_state(const STATE& s) {
	this->state = s;
}