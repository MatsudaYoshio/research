#include "stdafx.h"
#include "FrameRateCounter.h"

FrameRateCounter::FrameRateCounter(double dHistory)
{
	m_frameRate = 0.0;
	m_dHistory = dHistory;

	::QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	::QueryPerformanceCounter((LARGE_INTEGER*)&m_prevTime);
}


double FrameRateCounter::GetFrameRate()
{
	return m_frameRate;
}


void FrameRateCounter::SetFrameRate(double frameRate)
{
	m_frameRate = frameRate;
}

void FrameRateCounter::NewFrame()
{
	__int64 currTime;
	::QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	__int64 timeDiff64 = currTime - m_prevTime;
	double  timeDiff = timeDiff64 / static_cast<double>(m_frequency);

	if (timeDiff != 0)
	{
		m_frameRate = (1 / timeDiff)*(1 - m_dHistory) + m_dHistory*m_frameRate;
		m_prevTime = currTime;
	}
}