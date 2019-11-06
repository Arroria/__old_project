#include "MainHeader.h"
#include "MainProcess.h"



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	AF::g_processManager = new AF::ProcessManager(hInstance, nCmdShow);


	AF::g_processManager->SetWndProc(MainProcess::WndProc);
	AF::g_processManager->SetCallbackFunction(MainProcess::Initialize, MainProcess::Update, MainProcess::Render, MainProcess::Release);

	int	msg_wParam = NULL;
	if (SUCCEEDED( AF::g_processManager->CreateWnd(L"WorldWarT", WS_OVERLAPPEDWINDOW, 1280, 960, false) ) &&
		SUCCEEDED( AF::g_processManager->CreateDevice() ))
	{
		msg_wParam = AF::g_processManager->ProcessRun();

		AF::g_processManager->ReleaseDevice();
	}
	else
		return 0;


	SAFE_DELETE(AF::g_processManager);

	return (int)msg_wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



#include "MainGame.h"
MainGame*	g_mainGame	= nullptr;



HRESULT MainProcess::Initialize()
{
	g_mainGame = new MainGame();
	return g_mainGame->Initialize();
}

HRESULT MainProcess::Update()
{
	return g_mainGame->Update();
}

HRESULT MainProcess::Render()
{
	return g_mainGame->Render();
}

HRESULT MainProcess::Release()
{
	SAFE_RELEASE( g_mainGame );
	SAFE_DELETE( g_mainGame );
	return S_OK;
}

LRESULT MainProcess::WndProc(const HWND & hWnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	return g_mainGame ? g_mainGame->WndProc(hWnd, msg, wParam, lParam) : 0;
}

