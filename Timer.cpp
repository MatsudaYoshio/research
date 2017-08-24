#include <Windows.h>
#include <string>

#include "Timer.h"

Timer::Timer() { restart(); }

void  Timer::restart() {
	this->m_start = timeGetTime(); // �v���J�n���Ԃ�ۑ�
}

/* ���X�^�[�g����̃~���b�����b����Ԃ� */
double  Timer::elapsed(std::string unit) const { 
	DWORD end = timeGetTime();
	if (unit == "millsec") {
		return (double)(end - this->m_start);
	}
	else {
		return (double)(end - this->m_start) / 1000;
	}
}