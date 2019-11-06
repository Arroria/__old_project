#pragma once
class MainLoop
{
public:
	static bool Initialize();
	static void Update(double deltaTime);
	static bool Render();
	static bool Relase();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	MainLoop() {}
	~MainLoop() {}
};

