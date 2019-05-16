#include "Button.h"

void Button::setup(const ofRectangle& rect, const string& text, const ofPoint& text_position, const int text_size) {
	this->rect = rect;
	this->text = text;
	font.loadFont("./font/arial.ttf", text_size);
	this->text_position.x = rect.x + text_position.x;
	this->text_position.y = rect.y + text_position.y;
}

void Button::setup(const ofRectangle& rect, const string& text, const ofPoint& text_position, const int text_size, const ofColor& color) {
	this->color = color;
	this->setup(rect, text, text_position, text_size);
}

void Button::update() {
	switch (this->state) {
	case STATE::NO_PRESSED:
		this->alpha = ofColor::limit() / 3;
		break;
	case STATE::PRESSED:
		this->alpha = ofColor::limit();
		break;
	}
}

void Button::draw() const {
	ofSetColor(this->color, this->alpha);
	ofDrawRectRounded(this->rect, 10);
	ofSetColor(ofColor::black, this->alpha);
	this->font.drawString(this->text, this->text_position.x, this->text_position.y);
}

void Button::switch_state() {
	this->state = (this->state == STATE::PRESSED) ? STATE::NO_PRESSED : STATE::PRESSED;
}