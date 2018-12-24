// timer.h
// class CTimer
// ms 단위 Timer

// Windows 2000/XP 에서는 5ms 정도의 정확도를 가진다.

#ifndef __timer_h__
#define __timer_h__

class CTimer
{
private:
	int m_nStartTime;

public:
	CTimer();
	~CTimer();
	void start();
	int time();

	// time 시각으로부터 interval 만큼의 시간이 경과 했는가?
	bool elapsed(int &time, int interval);
};
#endif