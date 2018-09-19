#include "Mole.h"

using namespace param;

void Mole::setup(const ofRectangle& draw_rect) {
	this->draw_rect = draw_rect;
}

void Mole::update() {
}

void Mole::draw() {
	MOLE_IMAGE.draw(this->draw_rect);
}