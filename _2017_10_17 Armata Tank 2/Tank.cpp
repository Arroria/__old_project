#include "stdafx.h"

#include "Tank.h"
#include "MainLoop.h"

#define WND_SIZE_X 1280
#define WND_SIZE_Y 960
#define WND_POS_X ((GetSystemMetrics(SM_CXSCREEN) - WND_SIZE_X) * 0.5)
#define WND_POS_Y ((GetSystemMetrics(SM_CYSCREEN) - WND_SIZE_Y) * 0.5)
#define WND_STYLE WS_CAPTION // WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	g_processManager = new ProcessManager(hInstance);
	if (!g_processManager)
		return -1;

	WPARAM returnValue = -1;
	if (SUCCEEDED( g_processManager->CreateWnd(L"C'Tank", WND_STYLE, WndProc) ))
	{
		g_processManager->SetFunction(Initialize, Update, Render, Release);
		g_processManager->SetFPSLimite(60);
		g_processManager->WndResize(WND_SIZE_X, WND_SIZE_Y);
		g_processManager->WndMove(WND_POS_X, WND_POS_Y);
		if (SUCCEEDED( g_processManager->CreateDevice(true) ))
		{
			returnValue = g_processManager->Loop();
		}
	}
	delete g_processManager;
	return returnValue;
}