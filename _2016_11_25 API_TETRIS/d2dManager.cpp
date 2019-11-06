#include "stdafx.h"
#include "d2dManager.h"

d2dManager::d2dManager()
{
}

d2dManager::~d2dManager()
{
}

HRESULT d2dManager::init()
{
	//���̷�Ʈ 2d �̴�
	InitializeD2D();

	//����Ÿ�� �̴�
	InitializeRT();
	return S_OK;
}

void d2dManager::release()
{
	//deleteAll();
}

void d2dManager::begin(HDC getMemdc)
{
	RECT temp = { 0, 0, WINSIZEX, WINSIZEY };
	g_ipDCRT->BindDC(getMemdc, &temp);
	g_ipDCRT->BeginDraw();
}
void d2dManager::end()
{
	g_ipDCRT->EndDraw();
}

//�̹���

ID2D1Bitmap* d2dManager::addImage(string strKey, LPCWSTR fileName)
{
	//������ �̹��� �ִ��� ã�´�
	ID2D1Bitmap* img = findImage(strKey);

	//Ű���� ���� �̹����� �ִٸ� �߰����� �ʰ� �� �̹����� ����
	if (img) return img;

	//�̹��� ����
	img = CreateD2DBitmapFromFile(fileName);

	if (FAILED(CreateD2DBitmapFromFile(fileName)))
	{
		SAFE_DELETE(img);

		return NULL;
	}

	_mImageList.insert(make_pair(strKey, img));

	return img;
}


//�̹��� ã�� �Լ�
ID2D1Bitmap* d2dManager::findImage(string strKey)
{
	//�ش�Ű�� �ִ��� ���θ� �˻��Ѵ�
	mapD2ImageIter key = _mImageList.find(strKey);

	if (key != _mImageList.end())
	{
		return key->second;
	}

	return NULL;
}

//�̹��� ����� �Լ�
BOOL d2dManager::deleteImage(string strKey)
{
	//�ش� Ű���� �����ϴ��� �˻�
	mapD2ImageIter key = _mImageList.find(strKey);

	if (key != _mImageList.end())
	{
		//key->second->release();
		SAFE_DELETE(key->second);
		_mImageList.erase(key);

		return true;
	}

	return false;
}

BOOL d2dManager::deleteAll(void)
{
	mapD2ImageIter iter = _mImageList.begin();

	for (; iter != _mImageList.end();)
	{
		if (iter->second != NULL)
		{
			SAFE_DELETE(iter->second);
			iter = _mImageList.erase(iter);
		}
		else
		{
			++iter;
		}

	}

	_mImageList.clear();

	return TRUE;
}

void d2dManager::render(string strKey, int destX, int destY, float opacity)
{
	ID2D1Bitmap* img = findImage(strKey);
	if (img){
		::D2D1_RECT_F dxArea1 = ::D2D1::Rect(destX, destY, int(destX + img->GetSize().width), int(destY + img->GetSize().height));
		g_ipDCRT->DrawBitmap(img, dxArea1, opacity, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, (const D2D1_RECT_F*)0);
	}
}

void d2dManager::render(string strKey, int destX, int destY, int width, int height, float opacity)
{
	ID2D1Bitmap* img = findImage(strKey);
	if (img){
		::D2D1_RECT_F dxArea1 = ::D2D1::Rect(destX, destY, int(destX + width), int(destY + height));
		g_ipDCRT->DrawBitmap(img, dxArea1, opacity, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, (const D2D1_RECT_F*)0);
	}
}

void d2dManager::render(string strKey, int destX, int destY, float width, float height, float opacity)
{
	ID2D1Bitmap* img = findImage(strKey);
	if (img){
		::D2D1_RECT_F dxArea1 = ::D2D1::Rect(destX, destY, int(destX + img->GetSize().width * width), int(destY + img->GetSize().height * height));
		g_ipDCRT->DrawBitmap(img, dxArea1, opacity, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, (const D2D1_RECT_F*)0);
	}
}

void d2dManager::frameRender(string strKey, int destX, int destY, int currentFrameX, int currentFrameY,int maxFrameX,int maxFrameY,float opacity)
{
	ID2D1Bitmap* img = findImage(strKey);
	if (img){
		D2D1_RECT_F dxArea1 = D2D1::Rect(destX, destY, int(destX + img->GetSize().width/maxFrameX), int(destY + img->GetSize().height/maxFrameY));
		D2D1_RECT_F dxSour = D2D1::Rect(currentFrameX*img->GetSize().width / maxFrameX, currentFrameY*img->GetSize().height / maxFrameY, (currentFrameX + 1)*img->GetSize().width / maxFrameX, (currentFrameY + 1)*img->GetSize().height / maxFrameY);
		g_ipDCRT->DrawBitmap(img, dxArea1, opacity, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, dxSour);
	}
}

void d2dManager::LineRectangle(RECT rc, DWORD color, float alpha)
{
	ID2D1SolidColorBrush * brush = NULL;
	g_ipDCRT->CreateSolidColorBrush(D2D1::ColorF((float)GetRValue(color) / 256, (float)GetGValue(color) / 256, (float)GetBValue(color) / 256, alpha), &brush);
	D2D1_RECT_F rect;
	rect.left = rc.left;
	rect.top = rc.top;
	rect.right = rc.right;
	rect.bottom = rc.bottom;
	g_ipDCRT->DrawRectangle(&rect, brush);
}

void d2dManager::FillRectangle(RECT rc, DWORD color, float alpha)
{
	ID2D1SolidColorBrush * brush = NULL;
	g_ipDCRT->CreateSolidColorBrush(D2D1::ColorF((float)GetRValue(color) / 256, (float)GetGValue(color) / 256, (float)GetBValue(color) / 256, alpha), &brush);
	D2D1_RECT_F rect;
	rect.left = rc.left;
	rect.top = rc.top;
	rect.right = rc.right;
	rect.bottom = rc.bottom;
	g_ipDCRT->FillRectangle(&rect, brush);
}


//�ھ� �Լ�
void d2dManager::InitializeD2D(void)
{
	HRESULT hr = E_FAIL;

	//-----------------------------------------------------------------------
	//	D2D Factory �� �����Ѵ�.
	//-----------------------------------------------------------------------
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&g_ipD2DFactory);
	assert(hr == S_OK);

	//-----------------------------------------------------------------------
	//	Windows Imaging Component Factory �� �����Ѵ�.
	//-----------------------------------------------------------------------
	hr = ::CoCreateInstance(CLSID_WICImagingFactory,
		NULL, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&g_ipWICFactory));
	assert(hr == S_OK);
}

void d2dManager::InitializeRT()
{

	//assert(::g_ipDCRT == nullptr);

	HRESULT hr = E_FAIL;
	RECT rc = { 0, 0, WINSIZEX, WINSIZEY };


	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		D2D1_ALPHA_MODE_IGNORE),
		0,
		0,
		D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE,
		D2D1_FEATURE_LEVEL_DEFAULT
		);

	//hr = m_pD2DFactory->CreateDCRenderTarget(&props, &m_pDCRT);
	hr = g_ipD2DFactory->CreateDCRenderTarget(&props, &g_ipDCRT);
	assert(hr == S_OK);
}//���������

ID2D1Bitmap* d2dManager::CreateD2DBitmapFromFile(LPCWSTR pImageFullPath)//�̳��� ������
{
	assert(pImageFullPath != nullptr);
	assert(g_ipWICFactory != nullptr);

	HRESULT hr = E_FAIL;

	//----------------------------------------------------------------
	//	���ڴ��� �����Ѵ�.
	//----------------------------------------------------------------
	IWICBitmapDecoder* ipDecoderPtr = nullptr;
	hr = g_ipWICFactory->CreateDecoderFromFilename(pImageFullPath, nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&ipDecoderPtr);
	assert(hr == S_OK);

	//----------------------------------------------------------------
	//	���ڴ����� �������� ��´�.
	//----------------------------------------------------------------
	IWICBitmapFrameDecode* ipFramePtr = nullptr;
	hr = ipDecoderPtr->GetFrame(0, &ipFramePtr);
	assert(hr == S_OK);

	//----------------------------------------------------------------
	//	�������� ������� �ؼ� ���� �����͸� �����.
	//----------------------------------------------------------------
	//SAFE_DELETE:(g_ipConvertedSrcBmp);
	IWICFormatConverter*		g_ipConvertedSrcBmp;
	hr = g_ipWICFactory->CreateFormatConverter(&g_ipConvertedSrcBmp);
	assert(hr == S_OK);


	hr = g_ipConvertedSrcBmp->Initialize(ipFramePtr,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom);
	assert(hr == S_OK);

	//----------------------------------------------------------------
	//	����Ʈ�� �����͸� ������� �ؼ� ���� ��Ʈ���� �����.
	//----------------------------------------------------------------
	ID2D1Bitmap* g_ipD2DBitmap;
	//SAFE_DELETE(g_ipD2DBitmap);


	hr = g_ipDCRT->CreateBitmapFromWicBitmap(g_ipConvertedSrcBmp,
		nullptr,
		&g_ipD2DBitmap);
	assert(hr == S_OK);

	//SAFE_DELETE(ipDecoderPtr);
	//SAFE_DELETE(ipFramePtr);

	return g_ipD2DBitmap;
}