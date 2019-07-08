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
	case STATE::POINT:
		this->alpha = HALF_MAX_ALFHA;
		if (this->progress_state == this->item_rect.width) {
			pair<param::MENU_ITEM_ID, long long int> id(this->item_id, this->user_id);
			ofNotifyEvent(this->select_event, id);
			this->progress_state = 0;
			this->state = STATE::SELECTED;
		}
		else {
			this->progress_state = ofClamp(this->progress_state + this->progress_ratio*this->item_rect.width, 0, this->item_rect.width);
		}
		break;
	case STATE::INACTIVE:
		this->alpha = MAX_ALFHA;
	case STATE::SELECTED:
		this->progress_state = 0;
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

	/* プログレスバーの描画 */
	ofSetColor(MENU_ITEM_COLOR[static_cast<int>(this->item_id)], ofColor::limit());
	ofDrawRectangle(this->item_rect.getTopLeft(), this->progress_state, this->progress_bar_size);
}

bool MenuItem::is_inside(const int x, const int y) const {
	return this->item_rect.inside(x, y);
}

bool MenuItem::is_selected() const {
	return this->state == STATE::SELECTED;
}

void MenuItem::set_state(const STATE& s) {
	this->state = s;
}