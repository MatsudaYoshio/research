#include "Timer.h"

using namespace std::chrono;

void Timer::start() {
	this->m_start = system_clock::now();
}

int Timer::get_seconds() const {
	const auto diff = system_clock::now() - this->m_start;
	return duration_cast<seconds>(diff).count();
}

long long int Timer::get_milliseconds() const {
	const auto diff = system_clock::now() - this->m_start;
	return duration_cast<milliseconds>(diff).count();
}