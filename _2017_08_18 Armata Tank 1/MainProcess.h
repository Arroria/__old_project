#pragma once
class MainProcess
{
public:
	static	HRESULT	Initialize	();
	static	HRESULT	Update		();
	static	HRESULT	Render		();
	static	HRESULT	Release		();
	static	LRESULT	WndProc		(const HWND& hWnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);
};

