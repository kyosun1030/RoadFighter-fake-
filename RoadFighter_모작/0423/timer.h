// timer.h
// class CTimer
// ms ���� Timer

// Windows 2000/XP ������ 5ms ������ ��Ȯ���� ������.

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

	// time �ð����κ��� interval ��ŭ�� �ð��� ��� �ߴ°�?
	bool elapsed(int &time, int interval);
};
#endif