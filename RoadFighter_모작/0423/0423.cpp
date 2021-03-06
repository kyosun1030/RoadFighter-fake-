// 0423.cpp: 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "0423.h"
#include"GameMain.h"
#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

HWND hWnd;//window handler
HDC g_hdc;//dc handler
GameMain g_game;//게임 클래스
int width, height;
//const static PLAY Time = STOP;


int count;
// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY0423, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0423));

    MSG msg;

    // 기본 메시지 루프입니다.
    //while (GetMessage(&msg, nullptr, 0, 0))
    //{
    //    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    //    {
    //        TranslateMessage(&msg);
    //        DispatchMessage(&msg);
    //    }
    //}


	

	g_hdc = GetDC(hWnd);

	//새로운 메세지 루프
	GetMessage(&msg, 0, 0, 0);
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//gameloop()
			g_game.GameLoop(g_hdc);		
		}
	}

	ReleaseDC(hWnd, g_hdc);
    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0423));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_MY0423);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	
    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   int xxx = width / 2 - (XRES / 2);
   int yyy = width / 2 - (YRES);

   //hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
   //   xxx, yyy, XRES, YRES, nullptr, nullptr, hInstance, nullptr);
   hWnd = CreateWindowW(szWindowClass, szTitle, WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
	   xxx, yyy, XRES, YRES, nullptr, nullptr, hInstance, nullptr);
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//static HBITMAP hBitmap;
	//static int a = 0, b = 0;


    switch (message)
    {
	case WM_CREATE:
		//hBitmap = (HBITMAP)LoadImage(NULL, L"Image//bmp00001.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		g_game.ResourceInit(g_hdc);
		g_game.DataInit();
		GetClientRect(hWnd,&g_game.setClientRect());
		g_game.SetMyShipRect();
	//	SetTimer(hWnd, 1, 1000, 0);
		break;
	//case WM_TIMER:
	//	if (Time == START)//타이머가 시작되면 if문을 시작한다.
	//	{
	//		if (wParam == 1)//wParam이 TRUE가되면 if문을 실행한다.여기서는 wParam이 고유 식별번호 즉 ID이다.
	//		{
	//			if (count == 999)//카운터 999이 경우에 if문을 실행한다.
	//			{
	//				count = 0;//카운터를 0으로 초기화한다.
	//			}
	//		}count++;
	//		RECT r = { 896 + a, 100 + b, 987 + a, 330 + b };
	//		InvalidateRect(hWnd, &r, FALSE);
	//	}
	//	break;
	case WM_SIZE:
		GetClientRect(hWnd, &g_game.setClientRect());
		//GetClientRect(hWnd, &ttt);

		g_game.SetMyShipRect();		
		g_game.GameLoop(g_hdc);
		//count++;
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_LBUTTONDOWN:
		g_game.UpdateButon(WM_LBUTTONDOWN, lParam);
		break;
	case WM_MOUSEMOVE:
		g_game.UpdateButon(WM_MOUSEMOVE, lParam);
		break;
	case WM_LBUTTONUP:
		g_game.UpdateButon(WM_LBUTTONUP, lParam);
		break;

	case WM_KEYDOWN:
	 	
		g_game.SetKeyCheck(wParam, 30);
		switch (wParam)
		{

		case 0x20://(차량 가속하는 부분)
			/*if (g_game.speed > 0.01f && g_game.speed<20.0f)	
				g_game.speed +=0.25f;
*/
			
		case 0x59://"Y"
			if (g_game.GetExit())//참이면
			{
				PostQuitMessage(0);//게임 종료 
			}
			break;
		case 0x4E://"n"
			if (g_game.GetExit())
			{
				g_game.SetExit(FALSE);//
				g_game.m_bPause = FALSE;//일시정지풀고
			}

		case VK_ESCAPE:
			g_game.SetExit(TRUE);
			break;

		//case VK_F1:			
		case 0x70:
			g_game.m_bPause = !g_game.m_bPause;
			break;

		case 0x72:
			//g_game.cherInit();
			g_game.EnemyInit();
			g_game.DataInit();
			GetClientRect(hWnd, &g_game.setClientRect());
			g_game.SetMyShipRect();
			g_game.SetState(INTRO);
			g_game.speed = 0;
			g_game.m_uCount.m_speed = 0;
			g_game.m_uFual.m_fual = 101;		
		
			g_game.m_uEnemy.bDead = FALSE;
	
			break;
		default:
			break;
		}
		break;
	case WM_KEYUP:
		g_game.SetKeyCheck(wParam, 30);
		switch (wParam)
		{
		case 0x20:
		/*	if (g_game.speed >= 0.0f && g_game.speed <= 20.0f)
			g_game.speed -= 0.25f;*/
			
			break;
		case 0x72:
			//g_game.cherInit();
			g_game.EnemyInit();
			g_game.DataInit();
			GetClientRect(hWnd, &g_game.setClientRect());
			g_game.SetMyShipRect();
			g_game.SetState(INTRO);
			g_game.m_uEnemy.bDead = FALSE;

			break;
		default:
			break;
		}
		break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);

	
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
