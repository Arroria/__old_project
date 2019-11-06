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
	bool IsKeyUp		(const UINT& index)	const;
	bool IsKeyDown		(const UINT& index)	const;
	bool IsKeyNone		(const UINT& index)	const;
	bool IsKeyPressed	(const UINT& index)	const;

	POINT	GeMousePos() const;
	POINT	GeMouseDelta() const;
	int		GeMouseWheel() const;

public:
	void Begin();
	void End();
	void MsgProc(const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);

public:
	InputManager();
	~InputManager();
};

