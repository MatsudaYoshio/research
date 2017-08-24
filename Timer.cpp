#include <Windows.h>
#include <string>

#include "Timer.h"

Timer::Timer() { restart(); }

void  Timer::restart() {
	this->m_start = timeGetTime(); // 計測開始時間を保存
}

/* リスタートからのミリ秒数か秒数を返す */
double  Timer::elapsed(std::string unit) const { 
	DWORD end = timeGetTime();
	if (unit == "millsec") {
		return (double)(end - this->m_start);
	}
	else {
		return (double)(end - this->m_start) / 1000;
	}
}