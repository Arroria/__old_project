#include "stdafx.h"
#include "SaviorOfDokdo.h"

#include "LoopMain.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	g_processManager = new ProcessManager(hInstance);
	if (g_processManager &&
		SUCCEEDED( g_processManager->CreateWnd(L"SaviorOfDokdo", WS_POPUP, LoopMain::WndProc) ))
	{
		g_processManager->WndResize(1280, 960);
		g_processManager->WndMove(200, 100);
		g_processManager->SetFPSLimite(60);
		g_processManager->SetFunction(LoopMain::Initialize, LoopMain::Update, LoopMain::Render, LoopMain::Release);
		if (SUCCEEDED( g_processManager->CreateDevice(true) ))
		{
			g_processManager->Loop();
			g_processManager->GetDevice()->Release();
		}
	}
	return 0;
}

