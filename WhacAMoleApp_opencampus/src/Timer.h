#pragma once
#ifndef ___Class_Timer
#define ___Class_Timer

#include "ofMain.h"
#include <chrono>

class Timer {
private:
	std::chrono::system_clock::time_point m_start{ std::chrono::system_clock::time_point::min() };
public:
	void start();
	int get_seconds() const;
	long long int get_milliseconds() const;
};

#endif