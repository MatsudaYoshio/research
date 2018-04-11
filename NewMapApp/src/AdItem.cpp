#include "AdItem.h"

using namespace param;

void AdItem::setup(const ofRectangle& position, param::CONTENT_ID content_id) {
	this->position = position;
	this->content_id = content_id;
	this->image.load(CONTENT_DATA[static_cast<int>(content_id)].image_path);
	this->font.loadFont("meiryob.ttc", 35);
	this->df.setup(MENU_ITEM_COLOR[static_cast<int>(CONTENT_DATA[static_cast<int>(content_id)].menu_item_id)], this->frame_size, position);
}

void AdItem::draw() {
	this->df.draw();

	ofSetColor(ofColor::white);
	this->image.draw(this->position.getX() + this->frame_size, this->position.getY() + this->frame_size, this->position.getWidth() - (this->frame_size << 1), this->position.getHeight() - (this->frame_size << 1));

	ofSetColor(MENU_ITEM_COLOR[static_cast<int>(CONTENT_DATA[static_cast<int>(content_id)].menu_item_id)]);
	ofDrawCircle(this->position.getX() + 50, this->position.getY() + 50, 35);

	ofSetColor(ofColor::black);
	font.drawString(to_string(CONTENT_DATA[static_cast<int>(content_id)].number), this->position.getX() + 34, this->position.getY() + 65);
}