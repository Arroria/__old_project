#pragma once
#include "Singleton.h"

class InputManager :
	public Singleton<InputManager>
{
private:
	enum class KeyState
	{
		Up,
		Down,
		None,
		Pressed,
	};

private:
	std::array<KeyState, 256> m_keyState;
	POINT m_mousePos;
	POINT m_mouseDelta;
	UINT m_mouseWheel;
	bool m_wndActive;

public:
	bool IsKeyUp		(const UINT& index) const { return m_keyState[index] == KeyState::Up; }
	bool IsKeyDown		(const UINT& index) const { return m_keyState[index] == KeyState::Down; }
	bool IsKeyNone		(const UINT& index) const { return m_keyState[index] == KeyState::Up	|| m_keyState[index] == KeyState::None; }
	bool IsKeyPressed	(const UINT& index) const { return m_keyState[index] == KeyState::Down	|| m_keyState[index] == KeyState::Pressed; }

	POINT GetMousePos() const	{ return m_mousePos; }
	POINT GetMouseDelta() const	{ return m_mouseDelta; }
	UINT GetMouseWheel() const	{ return m_mouseWheel; }

	void SetMousePos(const int& x, const int& y);

public:
	void Begin();
	void End();
	void MsgProc(const HWND& hWnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);

public:
	InputManager();
	~InputManager();
};

