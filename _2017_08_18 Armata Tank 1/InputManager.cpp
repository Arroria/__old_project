#include "MainHeader.h"
#include "InputManager.h"

#include <windowsx.h>

namespace AF
{
	InputManager::InputManager()
		: m_isWndActive(true)
		, m_mousePos{ NULL, NULL }
		, m_mouseDelta{ NULL, NULL }
		, m_mouseWheel(NULL)
	{
		memset(m_keyInput, KeyValue::None, sizeof(m_keyInput));
	}
	InputManager::~InputManager()
	{
	}



	void InputManager::Initialize()
	{
		//EveryKey Update
		for (int id = 0; id < sizeof(m_keyInput) / sizeof(m_keyInput[0]); id++)
			UpdateKeyInput(id);
	}

	void InputManager::Update()
	{
		//EveryKey Update
		for (int id = 0; id < sizeof(m_keyInput) / sizeof(m_keyInput[0]); id++)
			UpdateKeyInput(id);
	}

	void InputManager::Reset()
	{
		m_mouseDelta.x = m_mouseDelta.y = 0;
		m_mouseWheel = 0;
	}

	bool InputManager::MsgProc(const HWND& hWnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam, LRESULT& returnValue)
	{
		switch (msg)
		{
		case WM_ACTIVATEAPP:
			m_isWndActive = wParam;
			returnValue = 0;
			return true;
			break;

		case WM_MOUSEMOVE:
		{
			POINT mousePrev = m_mousePos;
			m_mousePos.x = GET_X_LPARAM(lParam);
			m_mousePos.y = GET_Y_LPARAM(lParam);
			m_mouseDelta.x = mousePrev.x - m_mousePos.x;
			m_mouseDelta.y = mousePrev.y - m_mousePos.y;
		}
		returnValue = 0;
		return true;
		break;

		case WM_MOUSEWHEEL:
			m_mouseWheel = GET_WHEEL_DELTA_WPARAM(wParam);
			returnValue = 0;
			return true;
			break;
		}
		return false;
	}



	void InputManager::UpdateKeyInput(const int & keyID)
	{
		int& keyValue = m_keyInput[keyID];
		keyValue = m_isWndActive && GetAsyncKeyState(keyID) ?
			(keyValue == KeyValue::Pressed || keyValue == KeyValue::Down ? KeyValue::Pressed : KeyValue::Down) :
			(keyValue == KeyValue::None || keyValue == KeyValue::Up ? KeyValue::None : KeyValue::Up);
	}
}