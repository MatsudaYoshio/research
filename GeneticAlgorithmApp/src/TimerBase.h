#pragma once
#ifndef ___Class_TimerBase
#define ___Class_TimerBase

#include <chrono>

class TimerBase {
public:
	TimerBase(); // タイマをクリア
	void Clear(); // タイマをクリア
	bool IsStarted() const; // タイマ実行中ならtrueを返す
	void Start(); // タイマ開始
	unsigned long GetMs(); // タイマ開始後のミリ秒時間
private:
	std::chrono::system_clock::time_point m_start;
};

#endif