#ifndef ___Class_Timer
#define ___Class_Timer

#include <string>

class Timer {
private:
	DWORD m_start; // �v���J�n����
public:
	Timer();
	void  restart();
	double  elapsed(std::string unit = "sec") const; // ���X�^�[�g����̕b�����~���b����Ԃ�
};

#endif