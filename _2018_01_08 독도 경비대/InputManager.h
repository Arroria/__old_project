#pragma once
#include "Singleton.h"

class InputManager :
	public Singleton<InputManager>
{
private:
	int m_keyState[256];
	POINT m_mousePos;
	POINT m_mouseDelta;
	int m_mouseWheel;

	bool m_wndIsActive;

public:
	bool IsKeyUp(const int& key);
	bool IsKeyDown(const int& key);
	bool IsKeyNone(const int& key);
	bool IsKeyPressed(const int& key);

	POINT GetMousePos();
	POINT GetMouseDelta();
	int GetMouseWheel();

public:
	void Begin();
	void End();
	void MsgProc(const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);

public:
	InputManager();
	virtual ~InputManager();
};

