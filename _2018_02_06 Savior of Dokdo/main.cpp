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


	LRESULT rtn = -1;

	g_processManager = new ProcessManager(hInstance);
	if (g_processManager)
	{
		if (SUCCEEDED(g_processManager->CreateWnd(L"Savior of Dokdo", WS_POPUP, MainLoop::WndProc)))
		{
			g_processManager->SetFPSLimite(60);
			g_processManager->SetFunction(MainLoop::Initialize, MainLoop::Update, MainLoop::Render, MainLoop::Release);
			g_processManager->WndMove(160, 90);
			g_processManager->WndResize(1600, 900);
			if (SUCCEEDED(g_processManager->CreateDevice(true)))
			{
				rtn = g_processManager->Loop();
			}
		}
	}

	return (int)rtn;
}
