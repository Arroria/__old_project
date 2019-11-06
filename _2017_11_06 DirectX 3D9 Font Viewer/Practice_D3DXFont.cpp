#include "stdafx.h"
#include "Practice_D3DXFont.h"


#define WND_DEF_SIZE_X	(1000)
#define WND_DEF_SIZE_Y	(1000)
#define WND_DEF_POS_X	((GetSystemMetrics(SM_CXSCREEN) - WND_DEF_SIZE_X) * 0.5f)
#define WND_DEF_POS_Y	((GetSystemMetrics(SM_CYSCREEN) - WND_DEF_SIZE_Y) * 0.5f)


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
	if (SUCCEEDED( g_processManager->CreateWnd(L"Font Test", WS_POPUP, MainLoop::WndProc) ))
	{
		g_processManager->SetFPSLimite(60);
		g_processManager->WndResize(WND_DEF_SIZE_X, WND_DEF_SIZE_Y);
		g_processManager->WndMove(WND_DEF_POS_X, WND_DEF_POS_Y);
		g_processManager->SetFunction(MainLoop::Initialize, MainLoop::Update, MainLoop::Render, MainLoop::Relase);

		if (SUCCEEDED( g_processManager->CreateDevice(true) ))
		{
			returnValue = g_processManager->Loop();
		}
	}
	delete g_processManager;
	return (int)returnValue;
}

