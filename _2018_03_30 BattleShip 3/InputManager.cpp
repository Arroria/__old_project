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
	: m_mousePos{ 0, 0 }
	, m_mouseDelta{ 0, 0 }
	, m_mouseWheel(0)
	, m_isWndActivate(true)
{
	m_keyState.fill(KeyState::None);
}
InputManager::~InputManager()
{
}



void InputManager::Begin()
{
	POINT prevMousePos = m_mousePos;
	GetCursorPos(&m_mousePos);
	ScreenToClient(DXUTGetHWND(), &m_mousePos);
	//if		(m_mousePos.x < 0)			m_mousePos.x = 0;
	//else if (m_mousePos.x > WND_SIZE_X) m_mousePos.x = WND_SIZE_X;
	//if		(m_mousePos.y < 0)			m_mousePos.y = 0;
	//else if (m_mousePos.y > WND_SIZE_Y) m_mousePos.y = WND_SIZE_Y;

	m_mouseDelta.x = m_mousePos.x - prevMousePos.x;
	m_mouseDelta.y = m_mousePos.y - prevMousePos.y;



	auto KeyUpdate = [this](const UINT& index)
	{
		m_keyState[index] = m_isWndActivate && GetAsyncKeyState(index) ?
			(m_keyState[index] == KeyState::Pressed	|| m_keyState[index] == KeyState::Down	? KeyState::Pressed	: KeyState::Down) :
			(m_keyState[index] == KeyState::None	|| m_keyState[index] == KeyState::Up	? KeyState::None	: KeyState::Up);
	};
	for (UINT i = 0; i < 256; i++)
		KeyUpdate(i);
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
	case WM_ACTIVATE:
		m_isWndActivate = wParam == 1 || wParam == 2;
		break;

	case WM_MOUSEWHEEL:
		m_mouseWheel = GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	}
}



bool InputManager::IsKeyUp(const UINT & index) const		{ return m_keyState[index] == KeyState::Up; }
bool InputManager::IsKeyDown(const UINT & index) const		{ return m_keyState[index] == KeyState::Down; }
bool InputManager::IsKeyNone(const UINT & index) const		{ return m_keyState[index] == KeyState::None	|| m_keyState[index] == KeyState::Up; }
bool InputManager::IsKeyPressed(const UINT & index) const	{ return m_keyState[index] == KeyState::Pressed	|| m_keyState[index] == KeyState::Down; }

POINT InputManager::GeMousePos() const		{ return m_mousePos; }
POINT InputManager::GeMouseDelta() const	{ return m_mouseDelta; }
int InputManager::GeMouseWheel() const		{ return m_mouseWheel; }
