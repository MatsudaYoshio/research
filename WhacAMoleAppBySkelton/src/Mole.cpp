#include "Mole.h"

using namespace param;

void Mole::setup(const ofRectangle& draw_rect, const int hidden_time) {
	this->draw_rect = draw_rect;
	this->hidden_time = hidden_time;
}

void Mole::draw() {
	if (this->appearance_time != 0) { // �o�����Ԃ�0�łȂ��Ƃ��ɂ̓��O��������摜��`�悵�ďo��������B�����āA�o�����Ԃ����炷
		MOLE_IMAGE.draw(this->draw_rect);
		--this->appearance_time;
	}
	else if (this->hidden_time != 0) { // �B�ꎞ�Ԃ�0�łȂ��Ƃ��ɂ̓��O���̂Ȃ��摜�i���j��`�悷��B�����āA�B�ꎞ�Ԃ����炷
		HOLE_IMAGE.draw(this->draw_rect);
		--this->hidden_time;
	}
	else {
		HOLE_IMAGE.draw(this->draw_rect); // �ɂ��Ă���Ƃ��̓��O���̂Ȃ��摜�i���j��`�悷��B
	}
}

ofRectangle Mole::get_rect() const {
	return this->draw_rect;
}

bool Mole::is_idle() const {
	return this->appearance_time == 0 && this->hidden_time == 0;
}

bool Mole::is_appear() const {
	return this->appearance_time != 0;
}

void Mole::appear(const int appearance_time) {
	this->hidden_time = 0;
	this->appearance_time = appearance_time;
}

void Mole::hide(const int hidden_time) {
	this->appearance_time = 0;
	this->hidden_time = hidden_time;
}