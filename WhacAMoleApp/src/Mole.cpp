#include "Mole.h"

using namespace param;

void Mole::setup(const ofRectangle& draw_rect, const int hidden_time) {
	this->draw_rect = draw_rect;
	this->hidden_time = hidden_time;
}

void Mole::update() {

}

void Mole::draw() {
	if (this->appearance_time != 0) {
		MOLE_IMAGE.draw(this->draw_rect);
		--this->appearance_time;
	}
	else if (this->hidden_time != 0) {
		HOLE_IMAGE.draw(this->draw_rect);
		--this->hidden_time;
	}
	else {
		HOLE_IMAGE.draw(this->draw_rect);
	}
}

ofRectangle Mole::get_rect() const {
	return this->draw_rect;
}

bool Mole::is_idle() const {
	return this->appearance_time == 0 && this->hidden_time == 0;
}

void Mole::appear(const int appearance_time) {
	this->appearance_time = appearance_time;
}

void Mole::hide(const int hidden_time) {
	this->hidden_time = hidden_time;
}