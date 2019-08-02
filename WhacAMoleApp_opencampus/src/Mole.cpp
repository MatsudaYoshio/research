#include "Mole.h"

using namespace param;

random_device Mole::rd;
mt19937 Mole::mt(Mole::rd());
uniform_real_distribution<> Mole::random01(0.0, 1.0);
uniform_int_distribution<int> Mole::random_angry_time(50, 100);

long long int Mole::previous_star{ 0 };
long long int Mole::current_star{ 0 };

void Mole::setup(const ofRectangle& draw_rect, const int hidden_time) {
	this->draw_rect = draw_rect;
	this->hidden_time = hidden_time;
}

void Mole::update() {
	switch (state) {
	case STATE::HIDE:
		this->hidden_time = max(this->hidden_time - 1, 0);
		break;
	case STATE::NORMAL:
	case STATE::STAR:
		--this->appearance_time;
		if (!this->is_appear()) {
			if (this->random01(this->mt) < this->angry_probability) { // ˆê’èŠm—¦‚Å“{‚èó‘Ô‚É“ü‚é
				this->state = STATE::ANGRY;
				this->angry_time = this->random_angry_time(this->mt);
			}
			else {
				this->state = STATE::HIDE;
			}
		}
		break;
	case STATE::KING:
		--this->appearance_time;
		if (!this->is_appear()) {
			this->state = STATE::HIDE;
		}
		break;
	case STATE::ANGRY:
		this->appearance_time = 1;
		--this->angry_time;
		if (this->angry_time == 0) {
			pair<int, int> p(this->draw_rect.x, this->draw_rect.y);
			ofNotifyEvent(this->explosion_event, p);
		}
		break;
	case STATE::DAMEGE:
		this->appearance_time = 0;
		this->state = STATE::HIDE;
		break;
	}
}

void Mole::draw() {
	MOLE_IMAGE[static_cast<int>(this->state)].draw(this->draw_rect);
}

ofRectangle Mole::get_rect() const {
	return this->draw_rect;
}

bool Mole::is_idle() const {
	return this->state == STATE::HIDE && this->appearance_time == 0 && this->hidden_time == 0;
}

bool Mole::is_appear() const {
	return this->appearance_time != 0;
}

void Mole::appear(const int appearance_time) {
	this->hidden_time = 0;
	this->appearance_time = appearance_time;

	if (this->previous_star != this->current_star && this->current_star % this->king_threshold == 0) {
		this->state = STATE::KING;
		this->previous_star = this->current_star;
	}
	else {
		this->state = (this->random01(this->mt) < this->star_probability) ? STATE::STAR : STATE::NORMAL;
	}
}

void Mole::hide(const int hidden_time) {
	this->appearance_time = 0;
	this->hidden_time = hidden_time;
	this->state = STATE::HIDE;
}

void Mole::salvage() {
	switch (state) {
	case STATE::STAR:
		++this->current_star;
		this->score = this->star_score;
		break;
	case STATE::NORMAL:
	case STATE::ANGRY:
		this->score = this->normal_score;
		break;
	case STATE::KING:
		this->score = this->king_score;
		break;
	}

	this->state = STATE::DAMEGE;
}

int Mole::get_score() {
	const int s = this->score;
	this->score = 0;
	return s;
}