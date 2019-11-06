#include "DXUT.h"
#include "InputManager.h"


enum class InputManager::KeyState
{
	Up,
	Down,
	None,
	Pressed,
};

InputManager::InputManager()
{
	m_keyState.fill(KeyState::None);
	m_mousePos.x = m_mousePos.y = 0;
	m_mouseDelta.x = m_mouseDelta.y = 0;
	m_mouseWheel = 0;
	m_wndActive = true;
}
InputManager::~InputManager()
{
}



void InputManager::Begin()
{
	POINT mousePrev = m_mousePos;
	GetCursorPos(&m_mousePos);
	ScreenToClient(DXUTGetHWND(), &m_mousePos);
	m_mouseDelta.x = m_mousePos.x - mousePrev.x;
	m_mouseDelta.y = m_mousePos.y - mousePrev.y;

	auto KeyUpdate = [&, this](KeyState& inOut, const UINT& index)
	{
		inOut = ((m_wndActive && GetAsyncKeyState(index)) ?
			(inOut == KeyState::Pressed	|| inOut == KeyState::Down	? KeyState::Pressed	: KeyState::Down) :
			(inOut == KeyState::None	|| inOut == KeyState::Up	? KeyState::None	: KeyState::Up	));
	};
	for (UINT i = 0; i < 256; i++)
		KeyUpdate(m_keyState[i], i);
}

void InputManager::End()
{
	m_mouseDelta.x = m_mouseDelta.y= 0;
	m_mouseWheel = 0;
}

void InputManager::MsgProc(const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	switch (msg)
	{
	case WM_MOUSEWHEEL:
		m_mouseWheel = GET_WHEEL_DELTA_WPARAM(wParam);
		break;

	case WM_ACTIVATE:
		m_wndActive = wParam == 1 || wParam == 2;
		break;
	}
}




bool InputManager::IsKeyUp(const UINT & index) const		{ return m_keyState[index] == KeyState::Up; }
bool InputManager::IsKeyDown(const UINT & index) const		{ return m_keyState[index] == KeyState::Down; }
bool InputManager::IsKeyNone(const UINT & index) const		{ return m_keyState[index] == KeyState::None	|| m_keyState[index] == KeyState::Up; }
bool InputManager::IsKeyPressed(const UINT & index) const	{ return m_keyState[index] == KeyState::Pressed	|| m_keyState[index] == KeyState::Down; }

POINT InputManager::GetMousePos() const		{ return m_mousePos; }
POINT InputManager::GetMouseDelta() const	{ return m_mouseDelta; }
int InputManager::GetMouseWheel() const		{ return m_mouseWheel; }
