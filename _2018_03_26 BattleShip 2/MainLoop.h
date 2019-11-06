#pragma once
class MainLoop
{
public:
	static void Initialize();
	static void Update();
	static void Render();
	static void Release();
	static void MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

