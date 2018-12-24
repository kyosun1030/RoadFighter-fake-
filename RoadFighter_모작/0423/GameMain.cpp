#include "stdafx.h"
#include "GameMain.h"
#include <time.h>

#pragma comment(lib,"Msimg32.lib")//중요!

extern CTimer					g_Timer;


WCHAR test[20];
WCHAR speedcount[5000];
WCHAR fualcount[500];


GameMain::GameMain() :m_bPause(FALSE), m_timeTick(0), originy(0), speed(0), meterCount(0), countnum(0)
{	
	m_nGameState = INTRO;
	EnemyInit();
	m_uEnemy.m_Enegy=5;
	srand((unsigned int)time(NULL));
	m_nStageNum = 0;
	m_bExit = FALSE;

	m_uCount.m_speed = 0;
	m_uFual.m_fual = 101;
	

	m_SFrame.EndTick = GetTickCount();
	m_SFrame.startTick = m_SFrame.EndTick;
	m_SFrame.FrameRate = 0;
	m_SFrame.StartFramecount = 0;
	m_SFrame.frameCount = 0;
}


void GameMain::GameLoop(HDC hdc)
{
	DrawAll(hdc);
	Update();
}


void GameMain::DrawAll(HDC hdc)
{
	// TODO: 여기에 구현 코드 추가.

	switch (m_nGameState)
	{
	case INTRO:
		DrawBG();
		DrawPress();
		break;
	case GAME_RDY:
		DrawBaseBG();
		DrawSky();
		
		DrawEnemy();
		DrawMyShip();
		DrawRDY();
		DrawNum();
		DrawMNum();
		DrawHNum();
		break;

	case GAME_START:
		DrawBaseBG();
		DrawSky();
		DrawEnemy();
		DrawMyShip();
		DrawStart();
		DrawNum();
		DrawMNum();
		DrawHNum();
		break;

	case GAME_STAGE:
		DrawBaseBG();
		DrawSky();

		if (m_uMyship.bDead == FALSE)
		{
			DrawMyShip();
		}
		else
		{
			DrawExplode();
		}
		if (m_uEnemy.bDead == FALSE)
		{
			DrawEnemy();		
		}
		else
		{
			EDrawExplode();		
		}
	/*	if (m_uBullet.bShoot == TRUE)
		{
			DrawBullet();
		}*/
		
		DebugPrint(m_base.dc);
		Fakespeed(m_base.dc);
		FakeFual(m_base.dc);
		DrawNum();
		DrawMNum();
		DrawHNum();
		break;
	case GAME_OVER:
		DrawBaseBG();
		DrawSky();
		DrawExplode();
		DrawEnemy();
		DrawGameOver();
		break;
	case STAGE_CLEAR:
		DrawBaseBG();
		DrawSky();
		DrawMyShip();
		DrawClear();
		break;
	
	default:
		break;
	}
	if (m_bExit)
	{
		DrawExit();
	}
	BitBlt(hdc, 0, 0, m_base.bit.bmWidth, m_base.bit.bmHeight,
		m_base.dc, 0, 0, SRCCOPY);
}

void GameMain::Update()
{
	if (m_bExit)
	{
		m_bPause = TRUE;
	}
	if (m_bPause == TRUE) return;

	switch (m_nGameState)
	{
	case INTRO:
		UpDatePress();
		break;

	case GAME_RDY:
		
		UpdateRDY();
		break;
	case GAME_START:
		UpdateStart();
		break;

	case GAME_STAGE:
		UpdateNum();
		UpdateMNum();
		UpdateHNum();
	
		UpdataEnemy();
		UpdateFual();
		

		UpdateSky((float)speed);
		if (m_uMyship.bDead == TRUE)
		{
			UpdateExplode();
		
		}
		if (m_uEnemy.bDead == TRUE)
		{
			EUpdateExplode();
		}
	
		MultiMove(10);//기채 움직임
		break;
	case GAME_OVER:
		UpdateExplode();
		break;
	case STAGE_CLEAR:
		Updateclear();
		break;

	default:
		break;
	}		
 }


void GameMain::ResourceInit(HDC hdc)
{
	// TODO: 여기에 구현 코드 추가.
	m_base.bmp = (HBITMAP)LoadImage(NULL, L"Image//baseBg.bmp", 
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	GetObject(m_base.bmp, sizeof(BITMAP), &m_base.bit);

	m_base.dc = CreateCompatibleDC(hdc);
	m_base.Oldbmp =(HBITMAP)SelectObject(m_base.dc, m_base.bmp);

	//------------------------------------------------------------------
	m_BG.bmp = (HBITMAP)LoadImage(NULL, L"Image//title.bmp",
		IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE);
	m_BG.dc = CreateCompatibleDC(m_base.dc);//!!
	m_BG.Oldbmp = (HBITMAP)SelectObject(m_BG.dc, m_BG.bmp);


	//------------------------------------------------------배경
	m_Sky.bmp = (HBITMAP)LoadImage(NULL, L"Image//road.bmp",
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	m_Sky.dc = CreateCompatibleDC(m_base.dc);
	m_Sky.Oldbmp = (HBITMAP)SelectObject(m_Sky.dc, m_Sky.bmp);
	GetObject(m_Sky.bmp, sizeof(BITMAP), &m_Sky.bit);

	SetRect(&m_Sky.rt, 0, 0, m_Sky.bit.bmWidth, m_Sky.bit.bmHeight);

	//------------------------------------------------------------배경2

	m_Sky2.bmp = (HBITMAP)LoadImage(NULL, L"Image//road.bmp",
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	m_Sky2.dc = CreateCompatibleDC(m_base.dc);
	m_Sky2.Oldbmp = (HBITMAP)SelectObject(m_Sky2.dc, m_Sky2.bmp);
	GetObject(m_Sky2.bmp, sizeof(BITMAP), &m_Sky2.bit);

	SetRect(&m_Sky2.rt, 0, 0, m_Sky2.bit.bmWidth, m_Sky2.bit.bmHeight);


	//-------------------------------------------------------기체
	m_MyShip.bmp = (HBITMAP)LoadImage(NULL, L"Image//cars.bmp",
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_MyShip.dc = CreateCompatibleDC(m_base.dc);
	m_MyShip.Oldbmp = (HBITMAP)SelectObject(m_MyShip.dc, m_MyShip.bmp);
	GetObject(m_MyShip.bmp, sizeof(BITMAP), &m_MyShip.bit);

	SetRect(&m_MyShip.rt, 0, 0, m_MyShip.bit.bmWidth, m_MyShip.bit.bmHeight);

	//--------------------------------------------------------적기체
	m_Enemy.bmp = (HBITMAP)LoadImage(NULL, L"Image//truck.bmp",
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_Enemy.dc = CreateCompatibleDC(m_base.dc);
	m_Enemy.Oldbmp = (HBITMAP)SelectObject(m_Enemy.dc, m_Enemy.bmp);
	GetObject(m_Enemy.bmp, sizeof(BITMAP), &m_Enemy.bit);

	EnemyInit();
	
	//-------------------------------------------------------총알
	m_Bullet.bmp = (HBITMAP)LoadImage(NULL, L"Image//My_Bullet.bmp",
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_Bullet.dc = CreateCompatibleDC(m_base.dc);
	m_Bullet.Oldbmp = (HBITMAP)SelectObject(m_Bullet.dc, m_Bullet.bmp);
	GetObject(m_Bullet.bmp, sizeof(BITMAP), &m_Bullet.bit);

	SetRect(&m_Bullet.rt, 0, 0, m_Bullet.bit.bmWidth, m_Bullet.bit.bmHeight);

	//------------------------------------------------------------explod
	m_Explod.bmp = (HBITMAP)LoadImage(NULL, L"Image//explode.bmp",
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_Explod.dc = CreateCompatibleDC(m_base.dc);
	m_Explod.Oldbmp = (HBITMAP)SelectObject(m_Explod.dc, m_Explod.bmp);
	GetObject(m_Explod.bmp, sizeof(BITMAP), &m_Explod.bit);

	//------------------------------------------------------------------
	m_Press.bmp = (HBITMAP)LoadImage(NULL, L"image//PressAny.bmp",
			IMAGE_BITMAP,
			0, 0, LR_LOADFROMFILE);
	m_Press.dc = CreateCompatibleDC(m_base.dc);//!!
	m_Press.Oldbmp = (HBITMAP)SelectObject(m_Press.dc, m_Press.bmp);
	GetObject(m_Press.bmp, sizeof(BITMAP), &m_Press.bit);

	//m_Gameover
	m_Gameover.bmp = (HBITMAP)LoadImage(NULL, L"image//gameover.bmp",
		IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE);
	m_Gameover.dc = CreateCompatibleDC(m_base.dc);//!!
	m_Gameover.Oldbmp = (HBITMAP)SelectObject(m_Gameover.dc, m_Gameover.bmp);
	GetObject(m_Gameover.bmp, sizeof(BITMAP), &m_Gameover.bit);

	//clear m_Clear
	m_Clear.bmp = (HBITMAP)LoadImage(NULL, L"image//clear.bmp",
		IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE);
	m_Clear.dc = CreateCompatibleDC(m_base.dc);//!!
	m_Clear.Oldbmp = (HBITMAP)SelectObject(m_Clear.dc, m_Clear.bmp);
	GetObject(m_Clear.bmp, sizeof(BITMAP), &m_Clear.bit);
	//clear2
	m_Clear2.bmp = (HBITMAP)LoadImage(NULL, L"image//clear2.bmp",
		IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE);
	m_Clear2.dc = CreateCompatibleDC(m_base.dc);//!!
	m_Clear2.Oldbmp = (HBITMAP)SelectObject(m_Clear2.dc, m_Clear2.bmp);
	
	//m_Button
	m_Button.bmp = (HBITMAP)LoadImage(NULL, L"image//button_ALL.bmp",
		IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE);
	m_Button.dc = CreateCompatibleDC(m_base.dc);//!!
	m_Button.Oldbmp = (HBITMAP)SelectObject(m_Button.dc, m_Button.bmp);
	GetObject(m_Button.bmp, sizeof(BITMAP), &m_Button.bit);
	SetRect(&m_Button.rt, 0, 0, m_Button.bit.bmWidth / 4, m_Button.bit.bmHeight);


	int k = rand() % 10;


	m_Exit.bmp = (HBITMAP)LoadImage(NULL, L"image//exit.bmp",
		IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE);
	m_Exit.dc = CreateCompatibleDC(m_base.dc);//!!
	m_Exit.Oldbmp = (HBITMAP)SelectObject(m_Exit.dc, m_Exit.bmp);
	GetObject(m_Exit.bmp, sizeof(BITMAP), &m_Exit.bit);

	//---------------------------------------------RDY

	m_RDY.bmp = (HBITMAP)LoadImage(NULL, L"image//RDY.bmp",
		IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE);
	m_RDY.dc = CreateCompatibleDC(m_base.dc);//!!
	m_RDY.Oldbmp = (HBITMAP)SelectObject(m_RDY.dc, m_RDY.bmp);
	GetObject(m_RDY.bmp, sizeof(BITMAP), &m_RDY.bit);
	

	//-------------------------------------start
	m_Start.bmp = (HBITMAP)LoadImage(NULL, L"image//Start.bmp",
		IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE);
	m_Start.dc = CreateCompatibleDC(m_base.dc);//!!
	m_Start.Oldbmp = (HBITMAP)SelectObject(m_Start.dc, m_Start.bmp);
	GetObject(m_Start.bmp, sizeof(BITMAP), &m_Start.bit);

	//-----------------------------------count & num;
	m_Num.bmp = (HBITMAP)LoadImage(NULL, L"image//num.bmp",
		IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE);
	m_Num.dc = CreateCompatibleDC(m_base.dc);//!!
	m_Num.Oldbmp = (HBITMAP)SelectObject(m_Num.dc, m_Num.bmp);


	m_baseBG.bmp = (HBITMAP)LoadImage(NULL, L"Image//baseBG.bmp",
		IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE);
	m_baseBG.dc = CreateCompatibleDC(m_base.dc);//!!
	m_baseBG.Oldbmp = (HBITMAP)SelectObject(m_baseBG.dc, m_baseBG.bmp);



	m_Font = CreateFont(25, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial");
	OldFont = (HFONT)SelectObject(m_base.dc, m_Font);





}

void GameMain::DrawBG()
{
	BitBlt(m_base.dc, 0, 0, XRES, YRES, m_BG.dc, 0,
		0, SRCCOPY);
}

void GameMain::DrawBaseBG()
{
	BitBlt(m_base.dc, 0, 0, XRES, YRES, m_baseBG.dc, 0,
		0, SRCCOPY);
}

void GameMain::DrawSky()
{

	switch (m_nStageNum)
	{
	case 0:	StretchBlt(m_base.dc, 0, 0, XRES, YRES,
		m_Sky.dc, m_uSky.rt.left, m_uSky.rt.top,
		XRES, YRES, SRCCOPY);
		if (m_uCount.m_MNumFrame >1)
		{
			m_nGameState = STAGE_CLEAR;
			m_uCount.m_speed = 0;
			m_uFual.m_fual = 100;
			speed = 0;
		}
		break;

	case 1:
		StretchBlt(m_base.dc, 0, 0, XRES, YRES,
			m_Sky2.dc, m_uSky.rt.left, m_uSky.rt.top,
			XRES, YRES, SRCCOPY);
		if (m_uCount.m_MNumFrame >2)
		{
			m_nGameState = STAGE_CLEAR;
			m_uCount.m_speed = 0;
			m_uFual.m_fual = 100;
			speed = 0;
		}
		break;

	default:
		break;
	}
	//BitBlt(m_base.dc, 0, 0, XRES, YRES, m_BG.dc, 0, 0, SRCCOPY);
	
}


void GameMain::DrawMyShip()
{
	//BitBlt(m_base.dc, 0, 0, XRES, YRES, m_MyShip.dc, 0, 0, SRCCOPY);
	TransparentBlt(m_base.dc, m_MyShip.rt.left, m_MyShip.rt.top, m_MyShip.bit.bmWidth,
		m_MyShip.bit.bmHeight,m_MyShip.dc, 0, 0, m_MyShip.bit.bmWidth,
		m_MyShip.bit.bmHeight, RGB(255, 255, 255));
}

RECT & GameMain::setClientRect()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_rtClient;
}

void GameMain::SetMyShipRect()
{
	//m_MyShip.rt.left = m_rtClient.right /2 -m_MyShip.bit.bmWidth;
	//m_MyShip.rt.top = m_rtClient.bottom / 2 - 82;
	m_MyShip.rt.left = m_rtClient.right / 2 - (m_MyShip.bit.bmWidth+105);
	m_MyShip.rt.top = m_rtClient.bottom / 2 + 250;

	m_MyShip.rt.right = m_MyShip.rt.left + m_MyShip.bit.bmWidth;
	m_MyShip.rt.bottom = m_MyShip.rt.top + 82;
}

void GameMain::SetKeyCheck(unsigned int dir, int size)
{

	// TODO: 여기에 구현 코드 추가.
	if (m_nGameState == INTRO)
	{

		m_nGameState = GAME_RDY;
	}

	switch (dir)
	{
	case VK_LEFT:
		break;
	case VK_RIGHT:
		break;
	case VK_UP:
		break;
	case VK_DOWN:

		break;
	case VK_SPACE:
		break;


	default:
		break;
	}
}
	
void GameMain::UpdateSky(float size)
{

	m_uSky.m_EndTick = GetTickCount();
	if (m_uSky.m_EndTick - m_uSky.m_StartTick > size)
	{	
	
		m_uSky.m_StartTick = m_uSky.m_EndTick;
		m_uSky.rt.top -= size;
		m_uSky.rt.bottom = m_uSky.rt.top + m_Sky.bit.bmHeight;
	}
	
	//	m_uBG.rt.bottom -= size;
	if (m_uSky.rt.top < 0)
	{			
		m_uSky.rt.top = m_Sky.bit.bmHeight-YRES;
	}
}

void GameMain::UpdateCount()
{
	
	m_uCount.m_EndTick = GetTickCount();
	if (m_uCount.m_EndTick - m_uCount.m_StartTick>speed)
	{
		m_uCount.m_StartTick = m_uCount.m_EndTick;

		m_uCount.m_speed++;
	}

	if (m_uCount.m_speed > 100)
	{
		m_uCount.m_speed = 100;
	}
}

void GameMain::UpdateFual()
{
	m_uFual.m_EndTick = GetTickCount();
	if (m_uFual.m_EndTick - m_uFual.m_StartTick > 1500)
	{
		m_uFual.m_StartTick = m_uFual.m_EndTick;

		m_uFual.m_fual--;
	}

	if (m_uFual.m_fual <= 0)
	{
		m_nGameState = GAME_OVER;
	}
}


	
void GameMain::UpDataInit(UPDATE& up, DWORD aet, DWORD ast, DWORD mst, DWORD met,DWORD net, DWORD nst, DWORD mnet, DWORD mnst, DWORD hnet, DWORD hnst, DWORD ret, DWORD rst,
	int frame, int mf, int left, int top, int right, int bottom)
{
	up.a_EndTick = aet;
	up.a_StartTick = ast;

	up.m_EndTick = mst;
	up.m_StartTick = met;

	up.m_NstartTick = nst;
	up.m_NendTick = net;

	up.m_MNstartTick=mnst;
	up.m_MNendTick=mnet;

	up.m_HNstartTick = hnst;
	up.m_HNendTick = hnet;

	up.frame = frame;
	up.Maxframe = mf;
	SetRect(&up.rt, left, top, right, bottom);
}

POINT & GameMain::GetCenter(RECT rt)//모든 캐릭터들의 좌표의 중심점(피격판정)을 찾기위한 함수
{
	static POINT pt;

	pt.x=(rt.right - rt.left) / 2 + rt.left;
	pt.y = (rt.bottom - rt.top) / 2 + rt.top;

	return pt;
	// TODO: 여기에 반환 구문을 삽입합니다.
}

void GameMain::DataInit()
{
	UpDataInit(m_uSky);
	SetRect(&m_uSky.rt, 0, 0, XRES, YRES);

	UpDataInit(m_uEnemy);
	
	m_uEnemy.LRSTATE = 1;
	m_uEnemy.UDSTATE = 1;
	m_uEnemy.bCrash = FALSE;
	

	UpDataInit(m_uMyship);
	m_uMyship.bDead = FALSE;

	UpDataInit(m_UExplod);
	m_UExplod.Maxframe = 4;
	m_UExplod.frame = -1;

	UpDataInit(m_uPress);
	m_uPress.r_EndTick = GetTickCount();
	m_uPress.r_StartTick = m_uPress.r_EndTick;
	m_uPress.bDead = TRUE;

	UpDataInit(m_uRDY);
	m_uRDY.a_EndTick = GetTickCount();
	m_uRDY.a_StartTick = m_uRDY.a_EndTick;

	UpDataInit(m_uStart);
	m_uStart.a_EndTick = GetTickCount();
	m_uStart.a_StartTick = m_uStart.a_EndTick;

	UpDataInit(m_uButton);//karl20180427
	m_uButton.LRSTATE = DISABLE; //노말 , 클릭,하일라이트,비활성화
	UpDataInit(m_uClear);

	UpDataInit(m_uCount);
	m_uCount.m_NumFrame = -1;
	m_uCount.m_NumMaxFrame = 9;
	m_uCount.m_MNumFrame = -1;
	m_uCount.m_MNumMaxFrame = 9;
	m_uCount.m_HNumFrame = -1;
	m_uCount.m_HNumMaxFrame = 9;

}

int GameMain::DrawPress()
{
	if (m_uPress.bDead == TRUE) return 0;
	POINT pt = GetCenter(m_rtClient);
	// TODO: 여기에 구현 코드 추가.
	TransparentBlt(m_base.dc,
		pt.x - m_Press.bit.bmWidth / 2,
		pt.y - m_Press.bit.bmHeight / 2 + 200,
		m_Press.bit.bmWidth, m_Press.bit.bmHeight,
		m_Press.dc,
		0, 0,
		m_Press.bit.bmWidth,
		m_Press.bit.bmHeight,
		RGB(0, 0, 0));
	return 0;
}

int GameMain::UpDatePress()
{
	m_uPress.r_EndTick = GetTickCount();
	if (m_uPress.r_EndTick - m_uPress.r_StartTick > 500)
	{
		m_uPress.r_StartTick = m_uPress.r_EndTick;
		m_uPress.bDead = !m_uPress.bDead;
	}

	return 0;
}

int GameMain::UpdateButon(UINT msg, LPARAM lParam)
{
	// TODO: 여기에 구현 코드 추가.
	POINT pt;
	switch (msg)
	{
	case WM_LBUTTONDOWN:

		m_uButton.bCrash = TRUE;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		MouseExit(pt);
		if (PtInRect(&m_uButton.rt, pt))
		{
			m_uButton.LRSTATE = CLICKED;

		}
		break;
	case WM_LBUTTONUP:
		m_uButton.bCrash = FALSE;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		if (m_uButton.LRSTATE == CLICKED && PtInRect(&m_uButton.rt, pt))
		{
			PostQuitMessage(0);
		}
		m_uButton.LRSTATE = NORMAL;


		break;
	case WM_MOUSEMOVE:
		if (m_uButton.bCrash == FALSE)
		{
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			if (PtInRect(&m_uButton.rt, pt))
			{
				m_uButton.LRSTATE = HILIGHT;
			}
			else
			{
				m_uButton.LRSTATE = NORMAL;

			}
		}

		break;
	default:
		break;
	}
	return 0;
}

void GameMain::DebugPrint(HDC hdc)
{
	//wsprintf(test, L"Top:%d,bottom:%d", m_uSky.rt.top, m_uSky.rt.bottom, m_uSFrame.a_EndTick);
	//size_t pc;
	//StringCchLength(test, 40, &pc);

	//SetTextColor(hdc, RGB(255, 255, 255));
	//SetBkMode(hdc, TRANSPARENT);
	//TextOut(hdc, 2, 2, test, pc);

	//SetTextColor(hdc, RGB(0,0,0));
	//SetBkMode(hdc, TRANSPARENT);
	//TextOut(hdc, 0, 0, test, pc);


	////wsprintf(speedcount, L"Speed:%d", FSpeed, m_uSFrame.a_EndTick);
	////size_t SC;
	////StringCchLength(speedcount, 50, &SC);

	////SetTextColor(hdc, RGB(255, 255, 255));
	////SetBkMode(hdc, TRANSPARENT);
	////TextOut(hdc, 850, 600, speedcount, SC);
}

void GameMain::Fakespeed(HDC hdc)
{
		wsprintf(speedcount, L"Speed:%d",m_uCount.m_speed, m_uSFrame.a_EndTick);		
		
		size_t SC;
		StringCchLength(speedcount, 5000, &SC);

		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 850, 600, speedcount, SC);
}

void GameMain::FakeFual(HDC hdc)
{
	wsprintf(fualcount, L"Fual:%d", m_uFual.m_fual, m_uSFrame.a_EndTick);
	size_t SC;
	StringCchLength(fualcount, 5000, &SC);

	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, 850, 500, fualcount, SC);
}


void GameMain::DrawEnemy()
{
	
		TransparentBlt(m_base.dc, m_Enemy.rt.left, m_Enemy.rt.top, m_Enemy.bit.bmWidth, m_Enemy.bit.bmHeight,
			m_Enemy.dc, 0, 0, m_Enemy.bit.bmWidth, m_Enemy.bit.bmHeight, RGB(255, 255, 255));
	
}

void GameMain::UpdataEnemy()
{
	m_uEnemy.m_EndTick = GetTickCount();

		if (m_uEnemy.m_EndTick - m_uEnemy.m_StartTick > 10)
			//이미지의 위치갱신을 위해 100분의 1틱마다 갱신시켜준다.
		{
			m_uEnemy.m_StartTick = m_uEnemy.m_EndTick;
			POINT ptH;
			ptH = GetCenter(m_MyShip.rt);
			//피격판정이 될 플레이어 캐릭의 좌표의 중심
			m_uMyship.x = ptH.x;
			m_uMyship.y = ptH.y;

			ptH = GetCenter(m_Enemy.rt);
			//피격 판정이 될 장애물의 좌표중심
			m_uEnemy.x = ptH.x;
			m_uEnemy.y = ptH.y;

			if (ptH.y > 800)
				//해당 y축 범위 밖으로 나가면 사라지게 하기 위해 선언
			{
				m_uEnemy.bDead = TRUE;
				
			}

			if (ptH.y < 0)
				//장애물은 위애서 아래로만 내려오며 화면 밖에서 그려야 하기에 여기서는 죽지 않게 만들어 주었다.
			{
				m_uEnemy.bDead = FALSE;
			}

			if (PtInRect(&m_MyShip.rt, ptH))
				//플레이어 중심에 닿을경우 충돌판정이 일어나게 하기위해 가상의 사각형을 만들었다.
			{
				m_uMyship.bDead = TRUE;
				m_nGameState = GAME_OVER;				
			}
			

			//플레이어의 위치 좌표에 따라 LRSTATE나 UDSTATE의 값을 참 거짓으로 바꿔주는 부분이다. 참, 거짓 구분으로 장애물은 좌로 움직이기도 하고 우로 움직이기도 한다.
			if (m_uEnemy.x > m_uMyship.x)
			{
				m_Enemy.rt.left -= 2;
				m_Enemy.rt.right = m_Enemy.rt.left + m_Enemy.bit.bmWidth;
				m_uEnemy.LRSTATE = -1;

			}

			else if (m_uEnemy.x < m_uMyship.x)
			{
				m_Enemy.rt.left += 2;
				m_Enemy.rt.right = m_Enemy.rt.left + m_Enemy.bit.bmWidth;
				m_uEnemy.LRSTATE = 1;
			}

			if (m_uEnemy.y >= m_uMyship.y)
			{
				m_Enemy.rt.top += 5;
				m_Enemy.rt.bottom = m_Enemy.rt.top + m_Enemy.bit.bmHeight;
				m_uEnemy.UDSTATE = -1;
			}
			else if (m_uEnemy.y <= m_uMyship.y)
			{
				m_Enemy.rt.top += 5;
				m_Enemy.rt.bottom = m_Enemy.rt.top + m_Enemy.bit.bmHeight;
				m_uEnemy.UDSTATE = 1;
			}

			
			//탐색한 플레이어에게 이동하는 방식을 만들었다. defines.h에 인트형으로 상태를 저장하게 만들었으며 적과의 x y좌표가 같아질 때까지 움직인다.
			if (m_uEnemy.bCrash)
			{
				if (m_uEnemy.LRSTATE > 0)
				{
					m_Enemy.rt.left += 0;
					m_Enemy.rt.right = m_Enemy.rt.left + m_Enemy.bit.bmWidth;
				
				}
				else if (m_uEnemy.LRSTATE < 0)
				{
					m_Enemy.rt.left -= 0;				
					m_Enemy.rt.right = m_Enemy.rt.left + m_Enemy.bit.bmWidth;
				
				}
				if (m_uEnemy.UDSTATE > 0)
				{
					m_Enemy.rt.top += 5;
					m_Enemy.rt.bottom = m_Enemy.rt.top + m_Enemy.bit.bmHeight;
					
				}
				if (m_uEnemy.UDSTATE < 0)
				{
					m_Enemy.rt.top += 5;
					m_Enemy.rt.bottom = m_Enemy.rt.top + m_Enemy.bit.bmHeight;
				}
				return;
			}

			//단. 끝까지 따라오는것을 막기 위해 일정 범위까지 접근하면 bCrash상태가 되게 하여 아슬아슬한 정도까지 따라 붙게 된다.

			if (m_uEnemy.x > m_uMyship.x - 100 && m_uEnemy.x < m_uMyship.x + 100)
			{
				if (m_uEnemy.y > m_uMyship.y - 100 && m_uEnemy.y < m_uMyship.y)
				{
					m_uEnemy.bCrash = TRUE;
				}
			}
			
		}
}


void GameMain::UpdataEnemyex()
{
	m_uEnemy.m_EndTick = GetTickCount();
	if (m_uEnemy.m_EndTick - m_uEnemy.m_StartTick > 100)
	{
		m_uEnemy.m_StartTick = m_uEnemy.m_EndTick;

		m_Enemy.rt.top += 10;
		m_Enemy.rt.bottom = m_Enemy.rt.top + m_Enemy.bit.bmHeight;
	}
}

//void GameMain::DrawBullet()
//{	if(m_uBullet.bShoot)
//
//	
//	TransparentBlt(m_base.dc, m_Bullet.rt.left, m_Bullet.rt.top, m_Bullet.bit.bmWidth,
//	m_Bullet.bit.bmHeight, m_Bullet.dc, 0, 0, m_Bullet.bit.bmWidth,
//	m_Bullet.bit.bmHeight, RGB(0, 0, 0));	
//
//
//}
//
//void GameMain::SetBullet()
//{
//	m_Bullet.rt.left = m_MyShip.rt.right - 58;
//	m_Bullet.rt.top = m_MyShip.rt.top;
//	m_Bullet.rt.right = m_Bullet.rt.left + 10;
//	m_Bullet.rt.bottom = m_Bullet.rt.top + 30;
//}
//
//void GameMain::UpdateBullet(int size)
//{
//	if(m_uBullet.bShoot)
//	{
//		m_Bullet.rt.top -= size;		
//		m_Bullet.rt.bottom -= size;	
//		if (m_Bullet.rt.bottom <= 0)
//		{		
//			m_uBullet.bShoot = FALSE;
//			SetBullet();
//		}	
//
//		POINT pt;//총알의 점
//		pt.x = m_Bullet.rt.left + 5;
//		pt.y = m_Bullet.rt.top;
//		if (PtInRect(&m_Enemy.rt, pt))
//		{
//			m_uEnemy.m_Enegy--;
//
//			if (m_uEnemy.m_Enegy == 0)
//			{
//				m_uEnemy.bDead = TRUE;
//				
//				
//			//	MessageBox(NULL, L"착탄확인", L"check", MB_OK);
//			//	PostQuitMessage(0);
//			}
//			m_uBullet.bShoot = FALSE;
//			SetBullet();
//
//		}
//
//	}	
//}

void GameMain::cherInit()
{
	SetRect(&m_MyShip.rt, 0, 0, m_MyShip.bit.bmWidth, m_MyShip.bit.bmHeight);	
	//SetRect(&m_Bullet.rt, 0, 0, m_Bullet.bit.bmWidth, m_Bullet.bit.bmHeight);
}

void GameMain::EnemyInit()
{

	//SetRect(&m_Enemy.rt, 213, -140, m_Enemy.bit.bmWidth / 2, m_Enemy.bit.bmHeight / 2);

	int k = rand() % XRES - m_Enemy.bit.bmWidth;
	if (k < 213) k = 213;
	else if (k > 460) k = 460;
	SetRect(&m_Enemy.rt, k, -140, m_Enemy.bit.bmWidth / 2, m_Enemy.bit.bmHeight / 2);

	}
void GameMain::UpdateExplode()
{
	m_UExplod.a_EndTick = GetTickCount();
	if (m_UExplod.a_EndTick - m_UExplod.a_StartTick > 100)
	{
		m_UExplod.a_StartTick = m_UExplod.a_EndTick;
		m_UExplod.frame++;
		if (m_UExplod.frame > m_UExplod.Maxframe)
		{
			m_UExplod.frame = 0;
			
		}
	}
}

void GameMain::EUpdateExplode()
{
	m_UExplod.a_EndTick = GetTickCount();
	if (m_UExplod.a_EndTick - m_UExplod.a_StartTick > 100)
	{
		m_UExplod.a_StartTick = m_UExplod.a_EndTick;
		m_UExplod.frame++;
		if (m_UExplod.frame > m_UExplod.Maxframe)
		{
			m_UExplod.Maxframe = 0;
			m_uEnemy.bCrash = FALSE;
			EnemyInit();
			//m_nGameState = STAGE_CLEAR;
			//m_uEnemy.bDead = FALSE;			
			//UpdataEnemy();
			//cherInit();
			//m_uEnemy.m_Enegy += 5;
		}
		m_uEnemy.bDead = !m_uEnemy.bDead;
	}
}


void GameMain::DrawExplode()
{
	TransparentBlt(m_base.dc, m_MyShip.rt.left, m_MyShip.rt.top, m_MyShip.bit.bmWidth, m_MyShip.bit.bmHeight,
		m_Explod.dc, 100* m_UExplod.frame, 0, 100, 100, RGB(255, 0, 255));
}

void GameMain::EDrawExplode() {
	TransparentBlt(m_base.dc, m_Enemy.rt.left, m_Enemy.rt.top, m_Enemy.bit.bmWidth, m_Enemy.bit.bmHeight,
		m_Explod.dc, 100 * m_UExplod.frame, 0, 100, 100, RGB(255, 0, 255));
}

int GameMain::MultiMove(int size){//키 입력시 차량의 움직임과 연료감소를 구현한다.

	m_uMyship.m_EndTick = GetTickCount();
	if (m_uMyship.m_EndTick - m_uMyship.m_StartTick > 10)//이미지의 위치갱신을 위해 100분의 1틱마다 갱신시켜준다.
	{
		m_uMyship.m_StartTick = m_uMyship.m_EndTick;
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)//가상 키로 선언한 키가 눌렸을 때 발동
		{
			m_MyShip.rt.left -= 5;
			m_MyShip.rt.right = m_MyShip.rt.left + m_MyShip.bit.bmWidth;
			if (m_MyShip.rt.left < 213)
			{
				m_MyShip.rt.left = 213;//배경의 도로 이미지 밖으로 벗어나지 못하게 좌표 범위를 지정해준다.
				m_MyShip.rt.right = m_MyShip.rt.left + m_MyShip.bit.bmWidth;
			}
			m_uFual.m_EndTick = GetTickCount();
			if (m_uFual.m_EndTick - m_uFual.m_StartTick > 150)//움직일 때마다 연료가 달게 하기위해 틱카운트로 차이가 나면 연료가 떨어지게 만들었다.
			{
				m_uFual.m_StartTick = m_uFual.m_EndTick;

				m_uFual.m_fual--;
			}
			
		}
		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			m_MyShip.rt.top -= size;
			m_MyShip.rt.bottom = m_MyShip.rt.top + m_MyShip.bit.bmHeight;
			if (m_MyShip.rt.top < 30)
			{
				m_MyShip.rt.top = 30;
				m_MyShip.rt.bottom = m_MyShip.rt.top + m_MyShip.bit.bmHeight;
			}
			m_uFual.m_EndTick = GetTickCount();
			if (m_uFual.m_EndTick - m_uFual.m_StartTick > 100)//속도를 낼땐 더 많은 연료가 소모된다.
			{
				m_uFual.m_StartTick = m_uFual.m_EndTick;

				m_uFual.m_fual--;
			}
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)//오른쪽으로의 움직임은 왼쪽으로의 움직임과 반대로 해주면 된다.
		{
			m_MyShip.rt.left += 5;
			m_MyShip.rt.right = m_MyShip.rt.left + m_MyShip.bit.bmWidth;
			if (m_MyShip.rt.left > 500)
			{
				m_MyShip.rt.left = 500;
				m_MyShip.rt.right = m_MyShip.rt.left + m_MyShip.bit.bmWidth;
			}
			m_uFual.m_EndTick = GetTickCount();
			if (m_uFual.m_EndTick - m_uFual.m_StartTick > 150)//연료 소모량은 똑같게 해준다.
			{
				m_uFual.m_StartTick = m_uFual.m_EndTick;

				m_uFual.m_fual--;
			}
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			m_MyShip.rt.top += size;
			m_MyShip.rt.bottom = m_MyShip.rt.top + m_MyShip.bit.bmHeight;
			if (m_MyShip.rt.top > 680)
			{
				m_MyShip.rt.top = 680;
				m_MyShip.rt.bottom = m_MyShip.rt.top + m_MyShip.bit.bmHeight;
			}
			m_uFual.m_EndTick = GetTickCount();
			if (m_uFual.m_EndTick - m_uFual.m_StartTick > 200)//속도가 줄을 때는 소모되는 연료도 감소한다.
			{
				m_uFual.m_StartTick = m_uFual.m_EndTick;

				m_uFual.m_fual--;
			}
		}
		
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)//엑셀레이터 구현 부드럽게 가속하는걸 하기 위해 제작하였다.
		{ 		
			UpdateCount();
			if (speed >= 0.0f && speed<=30.0f)
				speed += 0.25f;			
		}
	}
	return 0;
}

//void GameMain::SetShoot(bool bs)
//{
//	m_uBullet.bShoot = bs;
//}
//
//BOOL GameMain::GetShoot()const
//{
//	return m_uBullet.bShoot;
//}


int GameMain::DrawGameOver()
{
	
	POINT pt = GetCenter(m_rtClient);

	TransparentBlt(m_base.dc,
		pt.x - m_Gameover.bit.bmWidth / 2-125,
		pt.y - m_Gameover.bit.bmHeight / 2,
		m_Gameover.bit.bmWidth, m_Gameover.bit.bmHeight,
		m_Gameover.dc,
		0, 0,
		m_Gameover.bit.bmWidth,
		m_Gameover.bit.bmHeight,
		RGB(0, 0, 0));

	return 0;
}

int GameMain::DrawClear()
{
	// TODO: 여기에 구현 코드 추가.
	POINT pt = GetCenter(m_rtClient);


	if (m_uClear.bCrash)
	{
		TransparentBlt(m_base.dc,
			pt.x - m_Clear.bit.bmWidth / 2,
			pt.y - m_Clear.bit.bmHeight / 2,
			m_Clear.bit.bmWidth, m_Clear.bit.bmHeight,
			m_Clear.dc,
			0, 0,
			m_Clear.bit.bmWidth,
			m_Clear.bit.bmHeight,
			RGB(255, 0, 255));
	}
	else
	{
		TransparentBlt(m_base.dc,
			pt.x - m_Clear.bit.bmWidth / 2,
			pt.y - m_Clear.bit.bmHeight / 2,
			m_Clear.bit.bmWidth, m_Clear.bit.bmHeight,
			m_Clear2.dc,
			0, 0,
			m_Clear.bit.bmWidth,
			m_Clear.bit.bmHeight,
			RGB(255, 0, 255));
	}
	return 0;
}

int GameMain::Updateclear()
{
	// TODO: 여기에 구현 코드 추가.
	m_uClear.a_EndTick= GetTickCount();
	if (m_uClear.a_EndTick - m_uClear.a_StartTick > 300)
	{
		m_uClear.a_StartTick = m_uClear.a_EndTick;
		m_uClear.bCrash = !m_uClear.bCrash;
		m_uClear.x++;

		if (m_uClear.x > 8)
		{
			m_nGameState = GAME_RDY;
			//cherInit();
			EnemyInit();
			DataInit();
			SetMyShipRect();
			m_uClear.x = 0;
			m_nStageNum++;
			if (m_nStageNum > 1) m_nStageNum = 0;
		}
	}
	return 0;
}
int GameMain::SetState(int state)
{
	// TODO: 여기에 구현 코드 추가.
	m_nGameState = state;

	return 0;
}

int GameMain::DrawExit()
{
	POINT pt = GetCenter(m_rtClient);
	BitBlt(m_base.dc,
		pt.x - m_Exit.bit.bmWidth / 2,
		pt.y - m_Exit.bit.bmHeight / 2,
		m_Exit.bit.bmWidth, m_Exit.bit.bmHeight,
		m_Exit.dc,
		0, 0, SRCCOPY);
	SetRect(&m_Exit.rt, pt.x - m_Exit.bit.bmWidth / 2,
		pt.y - m_Exit.bit.bmHeight / 2,
		pt.x - m_Exit.bit.bmWidth / 2 + m_Exit.bit.bmWidth,
		pt.y - m_Exit.bit.bmHeight / 2 + m_Exit.bit.bmHeight);
	return 0;
}

int GameMain::DrawRDY()
{
	POINT pt = GetCenter(m_rtClient);
	TransparentBlt(m_base.dc,
		pt.x - m_RDY.bit.bmWidth / 2-125,
		pt.y - m_RDY.bit.bmHeight / 2,
		m_RDY.bit.bmWidth, m_RDY.bit.bmHeight,
		m_RDY.dc,
		0, 0,
		m_RDY.bit.bmWidth,
		m_RDY.bit.bmHeight,
		RGB(255, 0, 255));
	return 0;
}

int GameMain::DrawStart()
{
	POINT pt = GetCenter(m_rtClient);
	TransparentBlt(m_base.dc,
		pt.x - m_Start.bit.bmWidth / 2-125,
		pt.y - m_Start.bit.bmHeight / 2,
		m_Start.bit.bmWidth, m_Start.bit.bmHeight,
		m_Start.dc,
		0, 0,
		m_Start.bit.bmWidth,
		m_Start.bit.bmHeight,
		RGB(255, 0, 255));	return 0;
}



int GameMain::DrawNum()
{
	TransparentBlt(m_base.dc, 930, 300, 50, 50,
		m_Num.dc, 100 * m_uCount.m_NumFrame, 0, 100, 100, RGB(0, 0, 0));
	return 0;
}
int GameMain::UpdateNum()
{
	m_uCount.m_NendTick = GetTickCount();
	if (m_uCount.m_NendTick - m_uCount.m_NstartTick > 1000)
	{
		m_uCount.m_NstartTick = m_uCount.m_NendTick;
		m_uCount.m_NumFrame++;

		if (m_uCount.m_NumFrame > m_uCount.m_NumMaxFrame)
		{
			m_uCount.m_NumFrame = 0;
		}
	}
	return 0;
}

int GameMain::DrawMNum()
{
	TransparentBlt(m_base.dc, 870, 300, 50, 50,
		m_Num.dc, 100 * m_uCount.m_MNumFrame, 0, 100, 100, RGB(0, 0, 0));
	return 0;
}
int GameMain::UpdateMNum()
{
	m_uCount.m_MNendTick = GetTickCount();
	if (m_uCount.m_MNendTick - m_uCount.m_MNstartTick > 10200)
	{
		m_uCount.m_MNstartTick = m_uCount.m_MNendTick;
		m_uCount.m_MNumFrame++;

		if (m_uCount.m_MNumFrame > m_uCount.m_MNumMaxFrame)
		{
			m_uCount.m_MNumFrame = 0;			
		}

		
	}	
	return 0;
}

int GameMain::DrawHNum()
{
	TransparentBlt(m_base.dc, 810, 300, 50, 50,
		m_Num.dc, 100 * m_uCount.m_HNumFrame, 0, 100, 100, RGB(0, 0, 0));
	return 0;
}
int GameMain::UpdateHNum()
{
	m_uCount.m_HNendTick = GetTickCount();
	if (m_uCount.m_HNendTick - m_uCount.m_HNstartTick > 100200)
	{
		m_uCount.m_HNstartTick = m_uCount.m_HNendTick;
		m_uCount.m_HNumFrame++;

		if (m_uCount.m_HNumFrame > m_uCount.m_HNumMaxFrame)
		{
			m_uCount.m_HNumFrame = 0;
		}
	}
	return 0;
}


int GameMain::UpdateRDY()
{
	m_uRDY.a_EndTick = GetTickCount();
	if (m_uRDY.a_EndTick - m_uRDY.a_StartTick > 3000)
	{
		m_uRDY.a_StartTick = m_uRDY.a_EndTick;

		m_nGameState = GAME_START;
		m_uStart.a_EndTick = GetTickCount();//tick의 초기화
		m_uStart.a_StartTick = m_uStart.a_EndTick;//tick의 초기화
	}
	return 0;
}

int GameMain::UpdateStart()
{
	m_uStart.a_EndTick = GetTickCount();
	if (m_uStart.a_EndTick - m_uStart.a_StartTick > 1500)
	{
		m_uStart.a_StartTick = m_uStart.a_EndTick;

		m_nGameState = GAME_STAGE;
		int mm = 0;
	}	return 0;
}


int GameMain::SetExit(BOOL a)
{
	m_bExit = a;
	return 0;
}

BOOL GameMain::GetExit()
{
	return m_bExit;
}

int GameMain::MouseExit(POINT pt)
{
	// TODO: 여기에 구현 코드 추가.
	RECT rtY;
	RECT rtN;
	SetRect(&rtY, 305 + m_Exit.rt.left, 165 + m_Exit.rt.top, 455 + m_Exit.rt.left, 213 + m_Exit.rt.top);
	SetRect(&rtN, 145 + m_Exit.rt.left, 165 + m_Exit.rt.top, 295 + m_Exit.rt.left, 213 + m_Exit.rt.top);

	if (PtInRect(&rtY, pt))
	{
		PostQuitMessage(0);
	}
	else if (PtInRect(&rtN, pt))
	{
		SetExit(FALSE);//
		m_bPause = FALSE;//일시정지풀고
	}
	return 0;	
}

GameMain::~GameMain()
{

}


//int GameMain::updateFrame()
//{
//	m_uSFrame.a_EndTick = GetTickCount();
//	if (m_uSFrame.a_EndTick-m_uSFrame.a_StartTick> 1000)
//	{
//		m_SFrame.FrameRate = (m_SFrame.frameCount - m_SFrame.frameCount);
//		m_uSFrame.a_StartTick = m_uSFrame.a_EndTick;	
//	}	
//	return 0;
//}

//int GameMain::UpdateFrameCount()
//{
//	wsprintf(test, L"FPS: %d", m_SFrame.FrameRate);
//}
//
//int GameMain::CountFrame()
//{
//	return 0;
//}
