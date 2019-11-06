#include "stdafx.h"
#include "ResourceMgr.h"


CResourceMgr::CResourceMgr()
	: m_nowDrawPos(VECTOR2(0, 0))
{
}
CResourceMgr::~CResourceMgr()
{
}



//초기화
void CResourceMgr::Init(const HWND& hWnd)
{
	m_hWnd = hWnd;
	m_hInstance = GetModuleHandle(NULL);

	m_showBufferDC = GetDC(m_hWnd);
	m_drawBufferDC = CreateCompatibleDC(m_showBufferDC);

	m_bitmap = CreateCompatibleBitmap(m_showBufferDC, WIN_SIZEX, WIN_SIZEY);
	SelectObject(m_drawBufferDC, m_bitmap);



	LoadBitmapTexture("BG_MainMenu",	L"./Resource/MainBackground/Main.bmp");
	LoadBitmapTexture("BG_Credit",		L"./Resource/MainBackground/Credit.bmp");
	LoadBitmapTexture("BG_InGame",		L"./Resource/MainBackground/InGame.bmp");


	LoadBitmapTexture("CHAR_Body",		L"./Resource/Character/Character_base.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Cloth_1",	L"./Resource/Character/Character_cloth_01.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Cloth_2",	L"./Resource/Character/Character_cloth_02.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Cloth_3",	L"./Resource/Character/Character_cloth_03.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Cloth_4",	L"./Resource/Character/Character_cloth_04.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Cloth_5",	L"./Resource/Character/Character_cloth_05.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Cloth_6",	L"./Resource/Character/Character_cloth_06.bmp"	, RGB(255, 0, 255));

	LoadBitmapTexture("CHAR_Eye_1",		L"./Resource/Character/Character_eyes_01.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Eye_2",		L"./Resource/Character/Character_eyes_02.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Eye_3",		L"./Resource/Character/Character_eyes_03.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Eye_4",		L"./Resource/Character/Character_eyes_04.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Eye_5",		L"./Resource/Character/Character_eyes_05.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Eye_6",		L"./Resource/Character/Character_eyes_06.bmp"	, RGB(255, 0, 255));

	LoadBitmapTexture("CHAR_Hair_1",	L"./Resource/Character/Character_hair_01.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Hair_2",	L"./Resource/Character/Character_hair_02.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Hair_3",	L"./Resource/Character/Character_hair_03.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Hair_4",	L"./Resource/Character/Character_hair_04.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Hair_5",	L"./Resource/Character/Character_hair_05.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Hair_6",	L"./Resource/Character/Character_hair_06.bmp"	, RGB(255, 0, 255));

	LoadBitmapTexture("CHAR_Mouth_1",	L"./Resource/Character/Character_mouth_01.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Mouth_2",	L"./Resource/Character/Character_mouth_02.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Mouth_3",	L"./Resource/Character/Character_mouth_03.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Mouth_4",	L"./Resource/Character/Character_mouth_04.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Mouth_5",	L"./Resource/Character/Character_mouth_05.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Mouth_6",	L"./Resource/Character/Character_mouth_06.bmp"	, RGB(255, 0, 255));

	LoadBitmapTexture("CHAR_Shoes_1",	L"./Resource/Character/Character_shoes_01.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Shoes_2",	L"./Resource/Character/Character_shoes_02.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Shoes_3",	L"./Resource/Character/Character_shoes_03.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("CHAR_Shoes_4",	L"./Resource/Character/Character_shoes_04.bmp"	, RGB(255, 0, 255));


	LoadBitmapTexture("UI_Main_Start",			L"./Resource/UI/Main UI_Start_Basics.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("UI_Main_Start_Click",	L"./Resource/UI/Main UI_Start_Click.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("UI_Main_Start_Clicked",	L"./Resource/UI/Main UI_Start_MouseOver.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("UI_Main_Credit",			L"./Resource/UI/Main UI_Cradit_Basics.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("UI_Main_Credit_Click",	L"./Resource/UI/Main UI_Cradit_Click.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("UI_Main_Credit_Clicked",	L"./Resource/UI/Main UI_Cradit_MouseOver.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("UI_Main_Exit",			L"./Resource/UI/Main UI_Exit_Basics.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("UI_Main_Exit_Click",		L"./Resource/UI/Main UI_Exit_Click.bmp"			, RGB(255, 0, 255));
		
	LoadBitmapTexture("UI_InGame_Home",				L"./Resource/UI/UI_HOME_Basics.bmp"			, RGB(255, 0, 255));
	LoadBitmapTexture("UI_InGame_Home_Click",		L"./Resource/UI/UI_HOME_Click.bmp"			, RGB(255, 0, 255));
	LoadBitmapTexture("UI_InGame_Home_Clicked",		L"./Resource/UI/UI_HOME_MouseOver.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("UI_InGame_Credit",			L"./Resource/UI/UI_CREDIT_Basics.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("UI_InGame_Credit_Click",		L"./Resource/UI/UI_CREDIT_Click.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("UI_InGame_Credit_Clicked",	L"./Resource/UI/UI_CREDIT_MouseOver.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("UI_InGame_Exit",				L"./Resource/UI/UI_EXIT_Basics.bmp"			, RGB(255, 0, 255));
	LoadBitmapTexture("UI_InGame_Exit_Click",		L"./Resource/UI/UI_EXIT_Click.bmp"			, RGB(255, 0, 255));
	LoadBitmapTexture("UI_InGame_Exit_Clicked",		L"./Resource/UI/UI_EXIT_MouseOver.bmp"		, RGB(255, 0, 255));


	LoadBitmapTexture("BT_Cloths",			L"./Resource/UI/UI_CLOTHES_Basics.bmp"			, RGB(255, 0, 255));
	LoadBitmapTexture("BT_Cloths_Click",	L"./Resource/UI/UI_CLOTHES_Click.bmp"			, RGB(255, 0, 255));
	LoadBitmapTexture("BT_Cloths_Clicked",	L"./Resource/UI/UI_CLOTHES_MouseOver.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("BT_Shoes",			L"./Resource/UI/UI_SHOES_Basics.bmp"			, RGB(255, 0, 255));
	LoadBitmapTexture("BT_Shoes_Click",		L"./Resource/UI/UI_SHOES_Click.bmp"				, RGB(255, 0, 255));
	LoadBitmapTexture("BT_Shoes_Clicked",	L"./Resource/UI/UI_SHOES_MouseOver.bmp"			, RGB(255, 0, 255));
	LoadBitmapTexture("BT_EyeBrow",			L"./Resource/UI/UI_EYES&BROW_Basics.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("BT_EyeBrow_Click",	L"./Resource/UI/UI_EYES&BROW_Click.bmp"			, RGB(255, 0, 255));
	LoadBitmapTexture("BT_EyeBrow_Clicked",	L"./Resource/UI/UI_EYES&BROW_MouseOver.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("BT_Mouth",			L"./Resource/UI/UI_MOUTH_Basics.bmp"			, RGB(255, 0, 255));
	LoadBitmapTexture("BT_Mouth_Click",		L"./Resource/UI/UI_MOUTH_Click.bmp"				, RGB(255, 0, 255));
	LoadBitmapTexture("BT_Mouth_Clicked",	L"./Resource/UI/UI_MOUTH_MouseOver.bmp"			, RGB(255, 0, 255));
	LoadBitmapTexture("BT_Hair",			L"./Resource/UI/UI_HAIR_Basics.bmp"				, RGB(255, 0, 255));
	LoadBitmapTexture("BT_Hair_Click",		L"./Resource/UI/UI_HAIR_Click.bmp"				, RGB(255, 0, 255));
	LoadBitmapTexture("BT_Hair_Clicked",	L"./Resource/UI/UI_HAIR_MouseOver.bmp"			, RGB(255, 0, 255));


	LoadBitmapTexture("ITEM_Cloth_1",	L"./Resource/UI/UI_Ingame_clothes_01.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Cloth_2",	L"./Resource/UI/UI_Ingame_clothes_02.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Cloth_3",	L"./Resource/UI/UI_Ingame_clothes_03.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Cloth_4",	L"./Resource/UI/UI_Ingame_clothes_04.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Cloth_5",	L"./Resource/UI/UI_Ingame_clothes_05.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Cloth_6",	L"./Resource/UI/UI_Ingame_clothes_06.bmp"	, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Eye_1",		L"./Resource/UI/UI_Ingame_eyes_01.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Eye_2",		L"./Resource/UI/UI_Ingame_eyes_02.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Eye_3",		L"./Resource/UI/UI_Ingame_eyes_03.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Eye_4",		L"./Resource/UI/UI_Ingame_eyes_04.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Eye_5",		L"./Resource/UI/UI_Ingame_eyes_05.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Eye_6",		L"./Resource/UI/UI_Ingame_eyes_06.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Hair_1",	L"./Resource/UI/UI_Ingame_hair_01.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Hair_2",	L"./Resource/UI/UI_Ingame_hair_02.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Hair_3",	L"./Resource/UI/UI_Ingame_hair_03.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Hair_4",	L"./Resource/UI/UI_Ingame_hair_04.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Hair_5",	L"./Resource/UI/UI_Ingame_hair_05.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Hair_6",	L"./Resource/UI/UI_Ingame_hair_06.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Mouth_1",	L"./Resource/UI/UI_Ingame_mouth_01.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Mouth_2",	L"./Resource/UI/UI_Ingame_mouth_02.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Mouth_3",	L"./Resource/UI/UI_Ingame_mouth_03.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Mouth_4",	L"./Resource/UI/UI_Ingame_mouth_04.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Mouth_5",	L"./Resource/UI/UI_Ingame_mouth_05.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Mouth_6",	L"./Resource/UI/UI_Ingame_mouth_06.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Shoes_1",	L"./Resource/UI/UI_Ingame_shoes_01.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Shoes_2",	L"./Resource/UI/UI_Ingame_shoes_02.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Shoes_3",	L"./Resource/UI/UI_Ingame_shoes_03.bmp"		, RGB(255, 0, 255));
	LoadBitmapTexture("ITEM_Shoes_4",	L"./Resource/UI/UI_Ingame_shoes_04.bmp"		, RGB(255, 0, 255));
}

//해제
void CResourceMgr::Release()
{
	auto iter = m_imageContainer.begin();
	auto iterBack = m_imageContainer.end();
	for (; iter != iterBack; iter++)
		DeleteDC(iter->second->bitmapDC);
	DeleteDC(m_drawBufferDC);
	DeleteDC(m_showBufferDC);

	DeleteObject(m_bitmap);
	DeleteObject(m_bitmapImage);
}



//파일로부터 비트맵 불러오기
CBitmapImage* CResourceMgr::LoadBitmapTexture(const std::string & key, const std::wstring & fileAddress)
{
	CBitmapImage*	image = new CBitmapImage();
	//LoadBitmap(m_hInstance, fileAddress.c_str());
	image->bitmap	= (HBITMAP)LoadImage(m_hInstance, fileAddress.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	image->bitmapDC	= CreateCompatibleDC(m_showBufferDC);

	BITMAP	imageInfo;
	GetObject(image->bitmap, sizeof(BITMAP), &imageInfo);

	image->width		= imageInfo.bmWidth;
	image->height		= imageInfo.bmHeight;
	image->isUseAlpha	= false;

	SelectObject(image->bitmapDC, image->bitmap);
	m_imageContainer.insert(std::make_pair(key, image));
	return image;
}

CBitmapImage* CResourceMgr::LoadBitmapTexture(const std::string & key, const std::wstring & fileAddress, const COLORREF & color)
{
	CBitmapImage* image = LoadBitmapTexture(key, fileAddress);
	image->isUseAlpha = true;
	image->alphaCode = color;
	return image;
}

CBitmapImage* CResourceMgr::GetImage(const std::string& key)
{
	auto iter = m_imageContainer.find(key);
	if (iter == m_imageContainer.end())
		return nullptr;

	return iter->second;
}



//렌더 준비
void CResourceMgr::Begin()
{
	Rectangle(m_drawBufferDC, -1, -1, WIN_SIZEX + 1, WIN_SIZEY + 1);
}

void CResourceMgr::SetWorld(const VECTOR2 & pos)
{
	m_nowDrawPos = pos;
}

void CResourceMgr::Draw(CBitmapImage*& image)
{
	if (!image->isUseAlpha)
	{
		BitBlt(m_drawBufferDC,
			m_nowDrawPos.x,
			m_nowDrawPos.y,
			image->width,
			image->height,
			image->bitmapDC, 0, 0, SRCCOPY);
	}
	else
	{
		TransparentBlt(m_drawBufferDC,
			m_nowDrawPos.x, m_nowDrawPos.y,
			image->width, image->height,
			image->bitmapDC,
			0, 0,
			image->width, image->height,
			image->alphaCode);
	}
}



void CResourceMgr::DrawString(const VECTOR2 & pos, const std::wstring& wstring, const int size, const COLORREF & color)
{
	SetBkMode(m_drawBufferDC, TRANSPARENT);
	SetTextColor(m_drawBufferDC, color);
	HFONT font = CreateFont(size, 0, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, VARIABLE_PITCH, L"FNT");
	SelectObject(m_drawBufferDC, font);
	TextOut(m_drawBufferDC, pos.x, pos.y, wstring.c_str(), wstring.size());
	DeleteObject(font);
}

//렌더 종료
void CResourceMgr::End()
{
	BitBlt(m_showBufferDC, 0, 0, WIN_SIZEX, WIN_SIZEY, m_drawBufferDC, 0, 0, SRCCOPY);
}
