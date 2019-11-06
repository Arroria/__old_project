#include "stdafx.h"
#include "KeyMgr.h"



CKeyMgr::CKeyMgr()
	: m_mouseDown(false)
	, m_mousePrevDown(false)
	, m_mousePos{ 0, 0 }
{
}
CKeyMgr::~CKeyMgr()
{
}



void CKeyMgr::Update()
{
	m_mousePrevDown = m_mouseDown;
	m_mouseDown = GetAsyncKeyState(VK_LBUTTON);
}

void CKeyMgr::MsgProc(const HWND & hWnd, const UINT & uMsg, const WPARAM & wParam, const LPARAM & lParam)
{
	switch (uMsg)
	{
	case WM_MOUSEMOVE:
		m_mousePos.x = (short)LOWORD(lParam);
		m_mousePos.y = (short)HIWORD(lParam);
		break;
	default:
		break;
	}
}
