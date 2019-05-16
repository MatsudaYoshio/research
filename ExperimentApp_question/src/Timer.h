#pragma once
#ifndef ___Class_Timer
#define ___Class_Timer

#include "ofMain.h"

#include <chrono>

class Timer {
private:
	std::chrono::system_clock::time_point m_start{ std::chrono::system_clock::time_point::min() };
	string minutes_string{ "00" }, seconds_string{ "00" };

	ofTrueTypeFont font;
	ofPoint view_position;
public:
	void setup(const ofPoint& view_position, int font_size);
	void update();
	void draw() const;

	void start();
	int get_seconds() const;
	int get_milliseconds() const;
};

#endif