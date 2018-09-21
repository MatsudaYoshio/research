#ifndef ___Class_FrameRateCounter
#define ___Class_FrameRateCounter

class FrameRateCounter
{
public:
	FrameRateCounter(double dHistory = 0.5);
	virtual ~FrameRateCounter(void) {};

	double GetFrameRate();
	void SetFrameRate(double frameRate);
	void NewFrame();

protected:
	double m_frameRate;
	double m_dHistory;
	__int64  m_frequency;
	__int64  m_lastTime;
	__int64  m_prevTime;
};

#endif