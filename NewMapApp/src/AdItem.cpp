#include "AdItem.h"

using namespace param;

void AdItem::setup(const ofRectangle& position, CONTENT_ID content_id) {
	this->position = position;
	this->content_id = content_id;
	this->image.load(CONTENT_DATA[static_cast<int>(content_id)].image_path);
	this->font.loadFont("meiryob.ttc", 35);
	this->df.setup(MENU_ITEM_COLOR[static_cast<int>(CONTENT_DATA[static_cast<int>(content_id)].menu_item_id)], this->frame_size, position);
	this->df_tmp.setup(MENU_ITEM_COLOR[static_cast<int>(CONTENT_DATA[static_cast<int>(content_id)].menu_item_id)], this->frame_size, position);
}

void AdItem::draw() {
	switch (this->state) {
	case STATE::NORMAL:
		this->df.draw();

		ofSetColor(ofColor::white);
		ofDrawRectangle(this->position.getX() + this->frame_size, this->position.getY() + this->frame_size, this->position.getWidth() - (this->frame_size << 1), this->frame_size + 80);

		this->image.draw(this->position.getX() + this->frame_size, this->position.getY() + this->frame_size + 80, this->position.getWidth() - (this->frame_size << 1), this->position.getHeight() - (this->frame_size << 1) - 80);

		ofSetColor(MENU_ITEM_COLOR[static_cast<int>(CONTENT_DATA[static_cast<int>(this->content_id)].menu_item_id)]);
		ofDrawCircle(this->position.getX() + 50, this->position.getY() + 50, 35);

		ofSetColor(ofColor::black);
		font.drawString(to_string(CONTENT_DATA[static_cast<int>(this->content_id)].number), this->position.getX() + 34, this->position.getY() + 65);

		if (CONTENT_DATA[static_cast<int>(this->content_id)].name.size() > 11) {
			font.drawString(CONTENT_DATA[static_cast<int>(this->content_id)].name.substr(0, 9) + "...", this->position.getX() + 90, this->position.getY() + 65);
		}
		else {
			font.drawString(CONTENT_DATA[static_cast<int>(this->content_id)].name, this->position.getX() + 90, this->position.getY() + 65);
		}

		break;
	case STATE::CAHANGE:
		if (this->alpha == ofColor::limit()) {
			this->content_id = move(this->content_id_tmp);
			this->df.change_color(MENU_ITEM_COLOR[static_cast<int>(CONTENT_DATA[static_cast<int>(this->content_id)].menu_item_id)]);
			this->image = move(this->image_tmp);
			this->alpha = 0;
			this->state = STATE::NORMAL;
			this->draw();
			break;
		}

		this->image_tmp.load(CONTENT_DATA[static_cast<int>(this->content_id_tmp)].image_path);
		ofColor c1{ MENU_ITEM_COLOR[static_cast<int>(CONTENT_DATA[static_cast<int>(this->content_id)].menu_item_id)] , ofColor::limit() - this->alpha };
		ofColor c2{ MENU_ITEM_COLOR[static_cast<int>(CONTENT_DATA[static_cast<int>(this->content_id_tmp)].menu_item_id)] , this->alpha };
		this->df.change_color(c1);
		this->df_tmp.change_color(c2);

		this->df.draw();

		ofSetColor(ofColor::white, ofColor::limit() - this->alpha);
		ofDrawRectangle(this->position.getX() + this->frame_size, this->position.getY() + this->frame_size, this->position.getWidth() - (this->frame_size << 1), this->frame_size + 80);

		this->image.draw(this->position.getX() + this->frame_size, this->position.getY() + this->frame_size + 80, this->position.getWidth() - (this->frame_size << 1), this->position.getHeight() - (this->frame_size << 1) - 80);

		ofSetColor(c1);
		ofDrawCircle(this->position.getX() + 50, this->position.getY() + 50, 35);

		ofSetColor(ofColor::black, ofColor::limit() - this->alpha);
		font.drawString(to_string(CONTENT_DATA[static_cast<int>(this->content_id)].number), this->position.getX() + 34, this->position.getY() + 65);

		if (CONTENT_DATA[static_cast<int>(this->content_id)].name.size() > 11) {
			font.drawString(CONTENT_DATA[static_cast<int>(this->content_id)].name.substr(0, 9) + "...", this->position.getX() + 90, this->position.getY() + 65);
		}
		else {
			font.drawString(CONTENT_DATA[static_cast<int>(this->content_id)].name, this->position.getX() + 90, this->position.getY() + 65);
		}

		this->df_tmp.draw();

		ofSetColor(ofColor::white, this->alpha);
		ofDrawRectangle(this->position.getX() + this->frame_size, this->position.getY() + this->frame_size, this->position.getWidth() - (this->frame_size << 1), this->frame_size + 80);

		this->image_tmp.draw(this->position.getX() + this->frame_size, this->position.getY() + this->frame_size + 80, this->position.getWidth() - (this->frame_size << 1), this->position.getHeight() - (this->frame_size << 1) - 80);

		ofSetColor(c2);
		ofDrawCircle(this->position.getX() + 50, this->position.getY() + 50, 35);

		ofSetColor(ofColor::black, this->alpha);
		font.drawString(to_string(CONTENT_DATA[static_cast<int>(this->content_id_tmp)].number), this->position.getX() + 34, this->position.getY() + 65);

		if (CONTENT_DATA[static_cast<int>(this->content_id_tmp)].name.size() > 11) {
			font.drawString(CONTENT_DATA[static_cast<int>(this->content_id_tmp)].name.substr(0, 9) + "...", this->position.getX() + 90, this->position.getY() + 65);
		}
		else {
			font.drawString(CONTENT_DATA[static_cast<int>(this->content_id_tmp)].name, this->position.getX() + 90, this->position.getY() + 65);
		}

		++this->alpha;

		break;
	}
}

void AdItem::change(CONTENT_ID content_id) {
	this->content_id_tmp = content_id;
	this->state = STATE::CAHANGE;
}