
#include "stdafx.h"
#include "Kinect_01.h"

#include "MainLoop.h"


HINSTANCE g_hInstance = NULL;
HWND g_hWnd = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	std::wstring wndName(L"이름을 안넣었네");

	//Create WndClass
	{
		WNDCLASSEXW wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KINECT_01));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_KINECT_01);
		wcex.lpszClassName = wndName.c_str();
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		RegisterClassExW(&wcex);


		if (! (g_hWnd = CreateWindowExW(0L,
			wndName.c_str(), wndName.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0,
			CW_USEDEFAULT, 0,
			nullptr, nullptr,
			hInstance, nullptr) ))
			return -1;

		ShowWindow(g_hWnd, nCmdShow);
	}



	if (FAILED( MainLoop::Initialize() ))
		return -1;
    
	MSG msg;
	PeekMessageW(&msg, NULL, NULL, NULL, PM_NOREMOVE);
	while (msg.message != WM_QUIT)
    {
		if (PeekMessageW(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			MainLoop::Update();
			MainLoop::Render();
		}
    }
	MainLoop::Release();

    return (int) msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
