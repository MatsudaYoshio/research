#include "Timer.h"

using namespace std::chrono;

void Timer::setup(const ofPoint& view_position, int font_size) {
	this->view_position = view_position;
	this->font.load("./font/arial.ttf", font_size);
}

void Timer::update() {
	const auto t = this->get_seconds();

	this->minutes_string = to_string(t / 60);
	if (this->minutes_string.size() == 1) {
		this->minutes_string = "0" + this->minutes_string;
	}

	this->seconds_string = to_string(t % 60);
	if (this->seconds_string.size() == 1) {
		this->seconds_string = "0" + this->seconds_string;
	}
}

void Timer::draw() const {
	ofSetColor(ofColor::black);
	this->font.drawString(this->minutes_string + ":" + this->seconds_string, this->view_position.x, this->view_position.y);
}

void Timer::start() {
	this->m_start = system_clock::now();
}

int Timer::get_seconds() const {
	const auto diff = system_clock::now() - this->m_start;
	return duration_cast<seconds>(diff).count();
}

int Timer::get_milliseconds() const {
	const auto diff = system_clock::now() - this->m_start;
	return duration_cast<milliseconds>(diff).count();
}