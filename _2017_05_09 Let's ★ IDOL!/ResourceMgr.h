#pragma once
#include "Singleton.h"
#include <map>
extern HWND g_hWnd;

struct CBitmapImage
{
	HBITMAP	bitmap;
	HDC		bitmapDC;
	int		width;
	int		height;
	bool		isUseAlpha;
	COLORREF	alphaCode;
};


class CResourceMgr :
	public CSingleton<CResourceMgr>
{
private:
	std::map<std::string, CBitmapImage*>	m_imageContainer;
	
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;

	HDC			m_showBufferDC;
	HDC			m_drawBufferDC;
	HBITMAP		m_bitmap;
	
	HBITMAP		m_bitmapImage;
	HDC			m_bitmapDC;

private:
	VECTOR2	m_nowDrawPos;

private:
	CBitmapImage* LoadBitmapTexture(const std::string& key, const std::wstring &fileAddress);
	CBitmapImage* LoadBitmapTexture(const std::string& key, const std::wstring &fileAddress, const COLORREF& color);

public:
	CBitmapImage*	GetImage(const std::string& key);

public:
	void	Init(const HWND& hWnd);
	void	Release();

	void	Begin();
	void	SetWorld(const VECTOR2& pos);
	void	Draw(CBitmapImage*& image);
	void	DrawString(const VECTOR2& pos, const std::wstring& wstring, const int size, const COLORREF& color = RGB(255, 255, 255));
	void	End();

public:
	CResourceMgr();
	~CResourceMgr();
};

#define ResourceMgr			SINGLETON_GET(CResourceMgr)
#define ResourceMgr_Release	SINGLETON_RELEASE(CResourceMgr)
#define Renderer ResourceMgr
