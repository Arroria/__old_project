#include "stdafx.h"
#include "InputManager.h"

enum KeyState
{
	Up,
	Down,
	None,
	Pressed
};

InputManager::InputManager()
	: m_keyState{ KeyState::None }
	, m_mousePos{ NULL }
	, m_mouseDelta{ NULL }
	, m_mouseWheel(NULL)
	, m_wndIsActive(true)
{
}
InputManager::~InputManager()
{
}


bool InputManager::IsKeyUp(const int & key)			{ return m_keyState[key] == KeyState::Up; }
bool InputManager::IsKeyDown(const int & key)		{ return m_keyState[key] == KeyState::Down; }
bool InputManager::IsKeyNone(const int & key)		{ return m_keyState[key] == KeyState::None		|| m_keyState[key] == KeyState::Up; }
bool InputManager::IsKeyPressed(const int & key)	{ return m_keyState[key] == KeyState::Pressed	|| m_keyState[key] == KeyState::Down; }

POINT InputManager::GetMousePos()	{ return m_mousePos; }
POINT InputManager::GetMouseDelta()	{ return m_mouseDelta; }
int InputManager::GetMouseWheel()	{ return m_mouseWheel; }

void InputManager::Begin()
{
	auto KeyUpdate = [this](const int& keyIndex)
	{
		int& keyState = this->m_keyState[keyIndex];
		keyState = (this->m_wndIsActive && GetAsyncKeyState(keyIndex)) ?
			(keyState == KeyState::Pressed || keyState == KeyState::Down ? KeyState::Pressed : KeyState::Down) :
			(keyState == KeyState::None || keyState == KeyState::Up ? KeyState::None : KeyState::Up);
	};

	for (int keyIndex = 0; keyIndex < 256; keyIndex++)
		KeyUpdate(keyIndex);
}

void InputManager::End()
{
	m_mouseDelta.x = m_mouseDelta.y = 0;
	m_mouseWheel = 0;
}

void InputManager::MsgProc(const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	switch (msg)
	{	
	case WM_MOUSEMOVE:
		POINT mousePrev = m_mousePos;
		m_mousePos.x = GET_X_LPARAM(lParam);
		m_mousePos.y = GET_Y_LPARAM(lParam);
		m_mouseDelta.x += m_mousePos.x - mousePrev.x;
		m_mouseDelta.y += m_mousePos.y - mousePrev.y;
		break;
	case WM_MOUSEWHEEL:
		m_mouseWheel = GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	case WM_ACTIVATE:
		m_wndIsActive = (wParam != 0);
		break;
	}
}
