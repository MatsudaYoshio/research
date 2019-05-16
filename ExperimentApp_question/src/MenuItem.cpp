#include "MenuItem.h"

using namespace param;

const array<string, MENU_ITEM_NUMBER> MenuItem::menu_item_name{ "  Sightseeing", "  Restaurants", "    Shopping", "      Hotels" };
const array<string, MENU_ITEM_NUMBER> MenuItem::menu_item_image_path{ "./fig/sightseeing_icon.png", "./fig/restaurant_icon.png", "./fig/shopping_icon.png", "./fig/hotel_icon.png" };

const ofPoint MenuItem::font_position{ 175, 390 };
const ofRectangle MenuItem::item_rect{ 175, 150, 250, 250 };
const ofRectangle MenuItem::texture_rect{ 175 + 250 * (1 - MenuItem::texture_size_ratio) / 2, 150 + 250 * (1 - MenuItem::texture_size_ratio) / 2, 250 * MenuItem::texture_size_ratio, 250 * MenuItem::texture_size_ratio };

void MenuItem::setup(const int item_id) {
	this->item_id = item_id;
	ofLoadImage(this->texture, this->menu_item_image_path[item_id]);
	this->font.loadFont("./font/meiryob.ttc", this->font_size);
}

void MenuItem::draw() const {
	/* €–Ú‚Ì”wŒi‚ð•`‰æ */
	ofFill();
	ofSetColor(MENU_ITEM_COLOR[this->item_id]);
	ofDrawRectangle(this->item_rect);

	/* ƒeƒNƒXƒ`ƒƒ‚Ì•\Ž¦*/
	ofSetColor(ofColor::white);
	this->texture.draw(this->texture_rect);

	/* €–Ú–¼‚ð•\Ž¦ */
	ofSetColor(ofColor::black);
	this->font.drawString(this->menu_item_name[this->item_id], this->font_position.x, this->font_position.y);
}