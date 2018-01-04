#ifndef ___Class_Timer
#define ___Class_Timer

#include <string>

class Timer {
private:
	DWORD m_start; // 計測開始時間
public:
	Timer();
	void  restart();
	double  elapsed(std::string unit = "sec") const; // リスタートからの秒数かミリ秒数を返す
};

#endif