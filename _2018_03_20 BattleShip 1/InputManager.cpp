#include "DXUT.h"
#include "InputManager.h"


InputManager::InputManager()
	: m_mouseWheel(0)
	, m_wndActive(true)
{
	m_keyState.fill(KeyState::None);
	m_mousePos.x = m_mousePos.y = 0;
	m_mouseDelta.x = m_mouseDelta.y = 0;
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

	auto KeyUpdate = [this](KeyState& place, const UINT& index)
	{
		place = ((m_wndActive && GetAsyncKeyState(index)) ?
			(place == KeyState::Down	 || place == KeyState::Pressed	? KeyState::Pressed	: KeyState::Down	) :
			(place == KeyState::Up		 || place == KeyState::None		? KeyState::None	: KeyState::Up		));
	};
	for (UINT i = 0; i < 256; i++)
		KeyUpdate(m_keyState[i], i);
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
		m_wndActive = wParam == 1 || wParam == 2;
		break;

	case WM_MOUSEHWHEEL:
		m_mouseWheel = GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	}
}



void InputManager::SetMousePos(const int & x, const int & y)
{
	POINT pos;
	pos.x = x;
	pos.y = y;
	m_mousePos = pos;
	ClientToScreen(DXUTGetHWND(), &pos);
	SetCursorPos(pos.x, pos.y);
}
