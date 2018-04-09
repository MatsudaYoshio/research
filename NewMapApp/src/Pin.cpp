#include "Pin.h"

using namespace param;

void Pin::setup(param::CONTENT_ID content_id) {
	this->content_id = content_id;
	this->x = CONTENT_DATA[static_cast<int>(content_id)].x;
	this->y = CONTENT_DATA[static_cast<int>(content_id)].y;
	this->pin_number = CONTENT_DATA[static_cast<int>(content_id)].number;
	this->color = MENU_ITEM_COLOR[static_cast<int>(CONTENT_DATA[static_cast<int>(content_id)].menu_item_id)];

	auto d{ this->size_ratio*this->radius };
	this->tx1 = this->x - d;
	this->tx2 = this->x + d;
	this->ty1 = this->ty2 = this->y + d;
	this->tx3 = this->x;
	this->ty3 = this->y + this->radius * 2;
	this->font_x = this->x - this->radius / 2.5;
	this->font_y = this->y + this->radius / 2.5;
	this->font.loadFont("meiryob.ttc", this->font_size);
}

void Pin::update() {

}

void Pin::draw() const {
	ofFill();
	ofSetColor(this->color);
	ofDrawCircle(this->x, this->y, this->radius);
	ofDrawTriangle(this->tx1, this->ty1, this->tx2, this->ty2, this->tx3, this->ty3);

	ofSetColor(ofColor::white);
	this->font.drawString(to_string(this->pin_number), this->font_x, this->font_y);
}