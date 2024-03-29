#include "stdafx.h"
#include "main.h"
#include "MainLoop.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	LRESULT returnV = -1;

	g_processManager = new ProcessManager(hInstance);
	if (SUCCEEDED( g_processManager->CreateWnd(L"Hurrah for the independent Korea", WS_POPUP, MainLoop::WndProc) ))
	{
		g_processManager->SetFPSLimite(60);
		g_processManager->SetFunction(MainLoop::Initialize, MainLoop::Update, MainLoop::Render, MainLoop::Release);
		g_processManager->WndResize(1600, 900);
		g_processManager->WndMove((1920 - 1600) * 0.5f, (1080 - 900) * 0.5f);
		if (SUCCEEDED( g_processManager->CreateDevice(true) ))
		{
			returnV = g_processManager->Loop();
		}
	}
	SAFE_DELETE(g_processManager);
	return (int)returnV;
}
