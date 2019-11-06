#pragma once

class d2dManager
{
public:
	typedef map<string, ID2D1Bitmap*> mapD2ImageList;			//이미지 리스트 맵
	typedef map<string, ID2D1Bitmap*>::iterator mapD2ImageIter;	//이미지 리스트 이터레이터

private:
	mapD2ImageList _mImageList;	//맵으로 구현된 이미지 리스트


	ID2D1Factory*				g_ipD2DFactory;
	ID2D1DCRenderTarget*		g_ipDCRT;
	IWICImagingFactory*			g_ipWICFactory;
public:
	HRESULT init();
	void release();


	void begin(HDC getMemdc);
	void end();

	//이미지 추가
	ID2D1Bitmap* addImage(string strKey, LPCWSTR fileName);


	////이미지 찾는 함수
	ID2D1Bitmap* findImage(string strKey);

	////이미지 지우는 함수
	BOOL deleteImage(string strKey);

	BOOL deleteAll(void);

	////렌더함수
	void render(string strKey, int destX, int destY, float opacity = 1.0);
	void render(string strKey, int destX, int destY, int width, int height, float opacity = 1.0f);
	void render(string strKey, int destX, int destY, float width, float height, float opacity = 1.0f);
	void frameRender(string strKey, int destX, int destY, int currentFrameX, int currentFrameY,int maxFrameX, int maxFrameY, float opacity = 1.0);

	void LineRectangle(RECT rc, DWORD color = RGB(0, 0, 0), float alpha = 1.f);
	void FillRectangle(RECT rc, DWORD color = RGB(0, 0, 0), float alpha = 1.f);

	//핵심
	void InitializeD2D();
	void InitializeRT();
	ID2D1Bitmap* CreateD2DBitmapFromFile(LPCWSTR pImageFullPath);

	d2dManager();
	~d2dManager();
};