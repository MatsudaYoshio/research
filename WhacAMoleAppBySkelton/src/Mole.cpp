#include "Mole.h"

using namespace param;

void Mole::setup(const ofRectangle& draw_rect, const int hidden_time) {
	this->draw_rect = draw_rect;
	this->hidden_time = hidden_time;
}

void Mole::draw() {
	if (this->appearance_time != 0) { // 出現時間が0でないときにはモグラがある画像を描画して出現させる。そして、出現時間を減らす
		MOLE_IMAGE.draw(this->draw_rect);
		--this->appearance_time;
	}
	else if (this->hidden_time != 0) { // 隠れ時間が0でないときにはモグラのない画像（穴）を描画する。そして、隠れ時間を減らす
		HOLE_IMAGE.draw(this->draw_rect);
		--this->hidden_time;
	}
	else {
		HOLE_IMAGE.draw(this->draw_rect); // 暇しているときはモグラのない画像（穴）を描画する。
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