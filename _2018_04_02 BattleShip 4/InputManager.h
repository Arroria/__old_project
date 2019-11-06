#pragma once
#include "Singleton.h"
class InputManager :
	public Singleton<InputManager>
{
	enum class KeyState;
private:
	std::array<KeyState, 256> m_keyState;
	POINT m_mousePos;
	POINT m_mouseDelta;
	int m_mouseWheel;
	bool m_isWndActivate;

public:
	bool IsKeyUp		(const UINT& index) const;
	bool IsKeyDown		(const UINT& index) const;
	bool IsKeyNone		(const UINT& index) const;
	bool IsKeyPressed	(const UINT& index) const;

	POINT GetMousePos	() const	{ return m_mousePos; }
	POINT GetMouseDelta	() const	{ return m_mouseDelta; }
	int	GetMouseWheel	() const	{ return m_mouseWheel; }

public:
	void Begin();
	void End();
	void MsgProc(const HWND& hWnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);

public:
	InputManager();
	~InputManager();
};

