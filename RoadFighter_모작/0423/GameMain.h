#pragma once

#include "defines.h"



class GameMain
{

public:
	GameMain();
	~GameMain();
	void GameLoop(HDC hdc);
	void DrawAll(HDC hdc);
	void Update();
	

public:
	UPDATE m_uSky;
	UPDATE m_uEnemy;
	UPDATE m_uEnemyex;
	UPDATE m_uMyship;
	UPDATE m_uBullet;
	UPDATE m_UExplod;
	UPDATE m_eUExplod;
	UPDATE m_bLives;
	UPDATE m_uPress;
	UPDATE m_uClear;
	UPDATE m_uButton;
	UPDATE m_uRDY;
	UPDATE m_uStart;
	UPDATE m_uSFrame;
	UPDATE m_uCount;
	UPDATE m_uFual;
private:
	HANDLES m_base;
	HANDLES m_BG;//인트로
	HANDLES m_MyShip;
	HANDLES m_Enemy;
	HANDLES m_Bullet;
	HANDLES m_Explod;
	HANDLES m_eExplod;	
	HANDLES m_Sky;//스크롤할 배경
	HANDLES m_Sky2;//스크롤할 배경
	HANDLES m_Press;//아무거나 누르세요

	HANDLES m_Gameover;
	HANDLES m_Clear;//karl20180430
	HANDLES m_Clear2;//karl20180430
	HANDLES m_Button;
	HANDLES m_Exit;
	HANDLES m_RDY;
	HANDLES m_Start;
	HANDLES m_Num;
	HANDLES m_baseBG;
	
	HFONT m_Font, OldFont;
	
	FRAME m_SFrame;


	RECT m_rtClient;
	

	//BOOL m_bShoot;


	int m_nGameState;//인트형 스테이트로 선언

public:

	void ResourceInit(HDC hdc);
	void DrawBG();
	void DrawBaseBG();
	void DrawSky();
	
	void DrawMyShip();
	RECT& setClientRect();
	void SetMyShipRect();
	//void SetEnemyRect();
	void UpdateSky(float size);

	void UpdateCount();

	void UpdateFual();

	void DataInit();

	void UpDataInit(UPDATE& up, DWORD aet = 0, DWORD ast = 0, DWORD mst = 0, DWORD met = 0, DWORD cet=0, DWORD cst=0, DWORD ret = 0, DWORD rst = 0,
		DWORD mnet=0, DWORD mnst=0, DWORD hnet=0, DWORD hnst=0, int frame = 0, int mf = 0,
		int left = 0, int top = 0, int right = 0, int bottom = 0);
	

	void SetKeyCheck(unsigned int dir, int size);
	POINT& GetCenter(RECT rt);
	void DebugPrint(HDC hdc);
	void Fakespeed(HDC hdc);

	void FakeFual(HDC hdc);

	void DrawEnemy();
	void UpdataEnemy();

	void UpdataEnemyex();
	
	//void DrawBullet();
	//void UpdateBullet(int size);
	//
	//void SetBullet();
	//BOOL GetMove() const;
	//void SetShoot(bool bs);
	//BOOL GetShoot() const;
	void cherInit();
	BOOL m_bPause;
	void EnemyInit();
	void UpdateExplode();
	void EUpdateExplode();
	void DrawExplode();
	void EDrawExplode();
	int MultiMove(int size);

	//void SetMove(bool bs);

	//void OnTimer(HWND hWnd);

	float speed;

	int FSpeed;

	int meterCount;

	int countnum;

	//void kill();

//	void Delay(clock_t n);


	int DrawGameOver();

	int DrawClear();

	int Updateclear();

	int SetState(int state);



public:
	int m_nStageNum;

	int m_timeTick;
	int originy;
	int DrawPress();
	int UpDatePress();

	int UpdateButon(UINT msg, LPARAM lParam);

	float secs;

private:
		BOOL m_bExit;
public:
	int SetExit(BOOL a);
	BOOL GetExit();
	int MouseExit(POINT pt);
	int DrawExit();

	int DrawRDY();
	int DrawStart();
	int DrawNum();

	int UpdateRDY();
	int UpdateStart();

	int UpdateNum();
	int DrawMNum();
	int UpdateMNum();
	int DrawHNum();
	int UpdateHNum();

	int updateFrame;
	int UpdateFrameCount;



	/*int m_NumFrame;
	int m_NumMaxFrame;

	int m_MNumFrame;
	int m_MNumMaxFrame;

	int m_HNumFrame;
	int m_HNumMaxFrame;*/

	HDC m_NumDC;
};

