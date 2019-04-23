#include "TimerBase.h"

using namespace std::chrono;

TimerBase::TimerBase() : m_start(system_clock::time_point::min()) {}

void TimerBase::Clear() {
	this->m_start = system_clock::time_point::min();
}

bool TimerBase::IsStarted() const {
	return (this->m_start.time_since_epoch() != system_clock::duration(0));
}

void TimerBase::Start() {
	this->m_start = system_clock::now();
}

unsigned long TimerBase::GetMs() {
	if (this->IsStarted()) {
		system_clock::duration diff;
		diff = system_clock::now() - this->m_start;
		return (unsigned)(duration_cast<milliseconds>(diff).count());
	}
	return 0;
}