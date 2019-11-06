#include "DXUT.h"
#include "InputManager.h"


enum class InputManager::KeyState
{
	Up,
	Down,
	None,
	Pressed
};

InputManager::InputManager()
	:  m_isWndActivate(true)
{
	m_keyState.fill(KeyState::None);

	GetCursorPos(&m_mousePos);
	ScreenToClient(DXUTGetHWND(), &m_mousePos);
	m_mouseDelta.x = m_mouseDelta.y = 0;
	m_mouseWheel = 0;
}
InputManager::~InputManager()
{
}



void InputManager::Begin()
{
	POINT prevMousePos = m_mousePos;
	GetCursorPos(&m_mousePos);
	ScreenToClient(DXUTGetHWND(), &m_mousePos);
	m_mouseDelta.x = m_mousePos.x - prevMousePos.x;
	m_mouseDelta.y = m_mousePos.y - prevMousePos.y;


	for (UINT i = 0; i < 256; i++)
		[&i, this]()
		{
			m_keyState[i] = ((m_isWndActivate && GetAsyncKeyState(i)) ?
				(m_keyState[i] == KeyState::Pressed	|| m_keyState[i] == KeyState::Down	? KeyState::Pressed	: KeyState::Down) :
				(m_keyState[i] == KeyState::None	|| m_keyState[i] == KeyState::Up	? KeyState::None	: KeyState::Up	));
		}();
}

void InputManager::End()
{
	m_mouseDelta.x = m_mouseDelta.y = 0;
	m_mouseWheel = 0;
}

void InputManager::MsgProc(const HWND & hWnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	switch (msg)
	{
	case WM_ACTIVATE:
		m_isWndActivate = wParam == 1 || wParam == 2;
		break;

	case WM_MOUSEWHEEL:
		m_mouseWheel = GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	}
}



bool InputManager::IsKeyUp		(const UINT & index) const	{ return m_keyState[index] == KeyState::Up; }
bool InputManager::IsKeyDown	(const UINT & index) const	{ return m_keyState[index] == KeyState::Down; }
bool InputManager::IsKeyNone	(const UINT & index) const	{ return m_keyState[index] == KeyState::None	|| m_keyState[index] == KeyState::Up; }
bool InputManager::IsKeyPressed	(const UINT & index) const	{ return m_keyState[index] == KeyState::Pressed	|| m_keyState[index] == KeyState::Down; }
