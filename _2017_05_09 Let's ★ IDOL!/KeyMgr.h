#pragma once
#include "Singleton.h"
class CKeyMgr :
	public CSingleton<CKeyMgr>
{
private:
	bool	m_mouseDown;
	bool	m_mousePrevDown;
	POINT	m_mousePos;

public:
	bool	MouseUp()		{ return !m_mouseDown &&  m_mousePrevDown; }
	bool	MouseDown()		{ return  m_mouseDown && !m_mousePrevDown; }
	bool	MouseNone()		{ return !m_mouseDown; }
	bool	MousePressed()	{ return  m_mouseDown; }

	POINT	GetMousePos()	{ return m_mousePos; }

public:
	void	Update();
	void	MsgProc(const HWND& hWnd, const UINT& uMsg, const WPARAM& wParam, const LPARAM& lParam);

public:
	CKeyMgr();
	~CKeyMgr();
};

#define KeyMgr			SINGLETON_GET(CKeyMgr)
#define KeyMgr_Release	SINGLETON_RELEASE(CKeyMgr)
