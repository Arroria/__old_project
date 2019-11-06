#pragma once

class d2dManager
{
public:
	typedef map<string, ID2D1Bitmap*> mapD2ImageList;			//�̹��� ����Ʈ ��
	typedef map<string, ID2D1Bitmap*>::iterator mapD2ImageIter;	//�̹��� ����Ʈ ���ͷ�����

private:
	mapD2ImageList _mImageList;	//������ ������ �̹��� ����Ʈ


	ID2D1Factory*				g_ipD2DFactory;
	ID2D1DCRenderTarget*		g_ipDCRT;
	IWICImagingFactory*			g_ipWICFactory;
public:
	HRESULT init();
	void release();


	void begin(HDC getMemdc);
	void end();

	//�̹��� �߰�
	ID2D1Bitmap* addImage(string strKey, LPCWSTR fileName);


	////�̹��� ã�� �Լ�
	ID2D1Bitmap* findImage(string strKey);

	////�̹��� ����� �Լ�
	BOOL deleteImage(string strKey);

	BOOL deleteAll(void);

	////�����Լ�
	void render(string strKey, int destX, int destY, float opacity = 1.0);
	void render(string strKey, int destX, int destY, int width, int height, float opacity = 1.0f);
	void render(string strKey, int destX, int destY, float width, float height, float opacity = 1.0f);
	void frameRender(string strKey, int destX, int destY, int currentFrameX, int currentFrameY,int maxFrameX, int maxFrameY, float opacity = 1.0);

	void LineRectangle(RECT rc, DWORD color = RGB(0, 0, 0), float alpha = 1.f);
	void FillRectangle(RECT rc, DWORD color = RGB(0, 0, 0), float alpha = 1.f);

	//�ٽ�
	void InitializeD2D();
	void InitializeRT();
	ID2D1Bitmap* CreateD2DBitmapFromFile(LPCWSTR pImageFullPath);

	d2dManager();
	~d2dManager();
};