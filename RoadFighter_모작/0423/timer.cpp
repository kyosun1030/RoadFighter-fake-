//timer.cpp

#include <windows.h>	//	������ �⺻ ����ü�� �Լ�
#include"stdafx.h"
#include "timer.h"

CTimer::CTimer()	//	������
{
	m_nStartTime = 0;	//	ó�� ���� �ð��� 0���� �ʱ�ȭ
}

CTimer::~CTimer()
{
}

void CTimer::start()	//	������ ������ ȣ��
{
	m_nStartTime = timeGetTime();	//timeGetTime()�� Win32 API �Լ�
							// ��� ���� ���� �� ��� �ð�(ms)
}

int CTimer::time()
{

	return (timeGetTime() - m_nStartTime);	//	���� �� ��� �ð� - ������ ������ �ð�
}

bool CTimer::elapsed(int &stime, int interval)
{
	int current_time = time();
	if (current_time >= stime + interval)	// ���������� �ش� ����� ��������( ex - �̻����� ����) ������ ����(interval) ��ŭ ������
		//#define MY_MISSILE_INTERVAL 240
	{
		stime = current_time;	//	���� �ð��� �̻����� �� �ð����� ����
		return true;	// �̻����� ��� �ְ� true
	}
	else	// ���ݸ�ŭ ��������
		return false;	//	�̻����� �� ��� �ɰ�� false
}
