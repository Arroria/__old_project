#include "stdafx.h"
#include "D3DX9_TEST.h"
#include "MainLoop.h"


// Program Handle
HINSTANCE		g_hInst;
HWND			g_hWnd;
CMainLoop*		g_pMainLoop;

int		g_mouseWheel = 0;
POINT	g_mousePos = {0};

HRESULT				CreateMainWindow	(int nCmdShow);
int					LoofMainWindow					();
LRESULT CALLBACK	WndProc				(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	//디버그 경고 메세지 무시 명령
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	g_hInst = hInstance;

	if (FAILED( CreateMainWindow(nCmdShow) ))
		return -1;
	return LoofMainWindow();
}


HRESULT CreateMainWindow(int nCmdShow)
{
	bool isFullScreen = MessageBoxW(g_hWnd, L"전체화면 사용 여부를 설정하셔야 합니다.\n전체화면으로 플레이 하시려면 예를 눌러 주십시오.", 0, MB_YESNO) == 6 ? true : false;

	//윈도우 크기 정의
	RECT rcClient = { 0, 0, WIN_SIZEX, WIN_SIZEY };
	if (isFullScreen)
	{
		AdjustWindowRect(&rcClient, WS_BORDER | WS_POPUP, true);
		DEVMODE devmode;
		ZeroMemory(&devmode, sizeof(devmode));
		devmode.dmBitsPerPel = 32;
		devmode.dmSize = sizeof(DEVMODE);
		devmode.dmPelsWidth = 800;
		devmode.dmPelsHeight = 600;
		devmode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		ChangeDisplaySettings(&devmode, CDS_FULLSCREEN);
	}
	else
		AdjustWindowRect(&rcClient, WIN_STYLE, FALSE);
	/*SetWindowPos(g_hWnd,
		NULL,
		WIN_POSX,
		WIN_POSY,
		rcClient.right - rcClient.left,
		rcClient.bottom - rcClient.top,
		SWP_NOZORDER
	);*/

	g_winSizeX = rcClient.right - rcClient.left;
	g_winSizeY = rcClient.bottom - rcClient.top ;

	WNDCLASSEX wndClassEx = { 0 };
	wndClassEx.cbSize			= sizeof(WNDCLASSEX);
	wndClassEx.style			= CS_HREDRAW | CS_VREDRAW;
	wndClassEx.lpfnWndProc		= WndProc;
	wndClassEx.cbClsExtra		= 0;
	wndClassEx.cbWndExtra		= 0;
	wndClassEx.hInstance		= g_hInst;
	wndClassEx.hIcon			= LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_D3DX9_TEST));
	wndClassEx.hCursor			= LoadCursor(nullptr, IDC_ARROW);
	wndClassEx.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wndClassEx.lpszMenuName		= NULL;
	wndClassEx.lpszClassName	= WIN_CLASSNAME;
	wndClassEx.hIconSm			= LoadIcon(wndClassEx.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassExW(&wndClassEx);


	//윈도우 정의
	g_hWnd = CreateWindow(
		WIN_CLASSNAME,
		WIN_TITLE,
		isFullScreen ? WS_BORDER | WS_POPUP : WIN_STYLE,
		WIN_POSX,
		WIN_POSY,
		isFullScreen ? 800 : g_winSizeX,
		isFullScreen ? 600 : g_winSizeY,
		nullptr,
		nullptr,
		g_hInst,
		nullptr);

	//윈도우 정의 실패
	if (!g_hWnd)
		return E_FAIL;


	ShowWindow(g_hWnd, nCmdShow);
	return S_OK;
}

int LoofMainWindow()
{
	g_pMainLoop = new CMainLoop();
	g_pMainLoop->Init();

    HACCEL hAccelTable = LoadAccelerators(g_hInst, MAKEINTRESOURCE(IDC_D3DX9_TEST));	//현재 이해불가 코드x0
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE);
    while (WM_QUIT != msg.message)
    {
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			if (!TranslateAccelerator(g_hWnd, hAccelTable, &msg))						//현재 이해불가 코드x1-0 + 함수가 실패를 반환해야만 조건문 안으로 들어감(이유 알수없음)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			g_pMainLoop->Update();
			g_pMainLoop->Render();
		}
    }

	SAFE_RELEASE(g_pMainLoop);
	SAFE_DELETE(g_pMainLoop);
    return (int) msg.wParam;
}

int CreateDevice(HWND hWnd, WPARAM wParam, LPARAM lParam);
int ResetDevice(HWND hWnd, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (GetAsyncKeyState(SHUTDOWN_KEY))
		PostQuitMessage(0);

	KeyMgr->MsgProc(hWnd, uMsg, wParam, lParam);

	switch (uMsg)
	{
	case WM_CREATE:
		return CreateDevice(hWnd, wParam, lParam);

	case WM_MOUSEWHEEL:
		g_mouseWheel -= (GET_WHEEL_DELTA_WPARAM(wParam) / 100.0f);
		break;

	case WM_CLOSE:
	case WM_DESTROY:
		return ResetDevice(hWnd, wParam, lParam);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}