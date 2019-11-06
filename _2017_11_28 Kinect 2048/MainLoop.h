#pragma once
class MainLoop
{
public:
	static bool Initialize();
	static void Update(double deltaTime);
	static bool Render();
	static bool Release();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	MainLoop() {}
	~MainLoop() {}
};

