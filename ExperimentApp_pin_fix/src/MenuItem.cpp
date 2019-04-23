#include "MenuItem.h"

using namespace param;

void MenuItem::setup(const string& image_path, const ofRectangle& item_rect, const string& item_name, const int font_x, const int font_y, const MENU_ITEM_ID item_id) {
	this->item_name = item_name;
	this->item_id = item_id;
	ofLoadImage(this->texture, image_path);
	this->item_rect.set(item_rect);
	auto texture_size{ this->item_rect.getWidth()*this->texture_size_ratio };
	this->texture_rect.set(this->item_rect.getCenter() - texture_size / 2, texture_size, texture_size);
	this->font.loadFont("./font/meiryob.ttc", this->font_size);
	this->font_x = font_x;
	this->font_y = font_y;
}

void MenuItem::update() {
	switch (this->state) {
	case STATE::ACTIVE:
		this->alpha = MAX_ALFHA;
		break;
	case STATE::INACTIVE:
		this->alpha = HALF_MAX_ALFHA;
		break;
	}
}

void MenuItem::update(const long long int user_id) {
	this->user_id = user_id;
	this->update();
}

void MenuItem::draw() const {
	/* 項目の背景を描画 */
	ofFill();
	ofSetColor(MENU_ITEM_COLOR[static_cast<int>(this->item_id)], this->alpha);
	ofDrawRectangle(this->item_rect);

	/* テクスチャの表示*/
	ofSetColor(ofColor::white, this->alpha);
	this->texture.draw(this->texture_rect);

	/* 項目名を表示 */
	ofSetColor(ofColor::black, this->alpha);
	this->font.drawString(this->item_name, this->font_x, this->font_y);
}

void MenuItem::set_state(const STATE& s) {
	this->state = s;
}

bool MenuItem::is_activated() const {
	return this->state == STATE::ACTIVE;
}