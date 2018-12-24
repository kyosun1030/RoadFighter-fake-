#pragma once
#include "stdafx.h"

#define XRES 1024
#define YRES 768
#define MAXSPEED 100

struct  HANDLES
{
	HBITMAP bmp,Oldbmp;
	HDC dc;
	BITMAP bit;
	RECT rt;
};

struct UPDATE
{
	DWORD m_StartTick;
	DWORD m_EndTick;
	DWORD a_StartTick;
	DWORD a_EndTick;

	DWORD r_StartTick;
	DWORD r_EndTick;

	
	DWORD m_NstartTick;
	DWORD m_NendTick;

	DWORD m_MNstartTick;
	DWORD m_MNendTick;

	DWORD m_HNstartTick;
	DWORD m_HNendTick;
	RECT rt;

	int frame = 0;
	int Maxframe = 8;

	int m_NumFrame = -1;
	int m_NumMaxFrame = 9;

	int m_MNumFrame = -1;
	int m_MNumMaxFrame = 9;

	int m_HNumFrame = -1;
	int m_HNumMaxFrame = 9;

	int x;
	int y;

	int LRSTATE;
	int UDSTATE;
	BOOL bCrash;
	BOOL bDead;
	BOOL bShoot;
	BOOL bMove;
	int	m_Enegy;	
	int m_speed = 0;

	int m_fual = 101;

};

enum GAME_STATE
{
	INTRO,
	GAME_STAGE,
	GAME_OVER,
	STAGE_CLEAR,
	GAME_RDY,
	GAME_START
};
enum PLAY { STOP = 0, START };
enum BUTTONS
{
	NORMAL = 0,
	CLICKED,
	HILIGHT,
	DISABLE
};


struct FRAME
{
	DWORD startTick;
	DWORD EndTick;
	int FrameRate;
	int frameCount;
	int StartFramecount;
};