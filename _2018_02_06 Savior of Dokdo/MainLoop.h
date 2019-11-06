#pragma once
class MainLoop
{
public:
	static HRESULT	Initialize();
	static void		Update(double deltaTime);
	static HRESULT	Render();
	static void		Release();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

