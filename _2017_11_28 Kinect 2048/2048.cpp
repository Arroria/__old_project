#include "stdafx.h"
#include "2048.h"
#include "MainLoop.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	g_processManager = new ProcessManager(hInstance);
	if (!g_processManager)
		return -1;

	WPARAM returnWParam = -1;
	if (SUCCEEDED( g_processManager->CreateWnd(L"2048", WS_POPUP, MainLoop::WndProc) ))
	{
		g_processManager->SetFPSLimite(60);
		g_processManager->SetFunction(MainLoop::Initialize, MainLoop::Update, MainLoop::Render, MainLoop::Release);
		g_processManager->WndResize(800, 1000);
		g_processManager->WndMove((GetSystemMetrics(SM_CXSCREEN) - 1000) * 0.5f, (GetSystemMetrics(SM_CYSCREEN) - 1000) * 0.5f);
		if (SUCCEEDED( g_processManager->CreateDevice(true) ))
			returnWParam = g_processManager->Loop();
	}
	SAFE_DELETE(g_processManager);
	return returnWParam;
}