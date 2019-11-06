#include "stdafx.h"
#include "ScInGame.h"

#include "ObjButton.h"
#include "Idol.h"

ScInGame::ScInGame()
	: m_nowSeletedType(0)
	, m_character(nullptr)
	, m_gameShutDown(false)
{
}
ScInGame::~ScInGame()
{
}



HRESULT ScInGame::Init()
{
	m_sceneBackGroundImage = ResourceMgr->GetImage("BG_InGame");

	m_character = new CIdol();
	m_character->Init();

	for (int i = 0; i < BT_UI_MENU::MENU_BT_END; i++)
	{
		m_uiMenuButton[i] = new ObjButton();
		m_uiMenuButton[i]->SetPos(VECTOR2(WIN_SIZEX - 64, 20 + i * 60));

		switch (i)
		{
			case BT_UI_MENU::HOME	:	m_uiMenuButton[i]->SetDefaultImageKey("UI_InGame_Home");	break;
			case BT_UI_MENU::EXIT	:	m_uiMenuButton[i]->SetDefaultImageKey("UI_InGame_Exit");	break;
			case BT_UI_MENU::CREDIT	:	m_uiMenuButton[i]->SetDefaultImageKey("UI_InGame_Credit");	break;
		}

		m_uiMenuButton[i]->Init();
	}
	for (int i = 0; i < BT_UI_KATEGORIE::KATEGORIE_BT_END; i++)
	{
		std::string	type;

		m_uiKategorieButton[i] = new ObjButton();
		m_uiKategorieButton[i]->SetPos(VECTOR2(80 + i * 80, 400 + 3));

		switch (i)
		{
			case BT_UI_KATEGORIE::CLOTHS	:	m_uiKategorieButton[i]->SetDefaultImageKey("BT_Cloths");	type = "ITEM_Cloth_";	break;
			case BT_UI_KATEGORIE::SHOES		:	m_uiKategorieButton[i]->SetDefaultImageKey("BT_Shoes");		type = "ITEM_Shoes_";	break;
			case BT_UI_KATEGORIE::EYES_BROW	:	m_uiKategorieButton[i]->SetDefaultImageKey("BT_EyeBrow");	type = "ITEM_Eye_";		break;
			case BT_UI_KATEGORIE::MOUTH		:	m_uiKategorieButton[i]->SetDefaultImageKey("BT_Mouth");		type = "ITEM_Mouth_";	break;
			case BT_UI_KATEGORIE::HAIR		:	m_uiKategorieButton[i]->SetDefaultImageKey("BT_Hair");		type = "ITEM_Hair_";	break;
		}

		m_uiKategorieButton[i]->Init();


		char num[10];
		for (int j = 0; j < (j == BT_UI_KATEGORIE::SHOES ? 4 : 6); j++)
		{
			m_itemButton[i][j] = new ObjButton();
			m_itemButton[i][j]->SetClickImageOff();
			m_itemButton[i][j]->SetClickedImageOff();
			int jojung;
			switch (j)
			{
				case 0: jojung = 0; break;
				case 1: jojung = 5; break;
				case 2: jojung = 5; break;
				case 3: jojung = 10; break;
				case 4: jojung = 0; break;
				case 5: jojung = -3; break;
			}
			m_itemButton[i][j]->SetPos(VECTOR2(50 + j * 125 + jojung, 470));
			m_itemButton[i][j]->SetDefaultImageKey(type + itoa(j + 1, num, 10));
			m_itemButton[i][j]->Init();
		}
	}


	return S_OK;
}

void ScInGame::Update()
{
	for (int i = 0; i < BT_UI_MENU::MENU_BT_END; i++)
	{
		m_uiMenuButton[i]->Update();

		if (m_uiMenuButton[i]->GetButtonIsClicked())
		{
			switch (i)
			{
			case BT_UI_MENU::HOME:		SceneMgr->ChangeScene("MainMenu");	break;
			case BT_UI_MENU::EXIT:		m_gameShutDown = true;	break;
			case BT_UI_MENU::CREDIT:	SceneMgr->ChangeScene("Credit");	break;
			}
		}
	}


	for (int i = 0; i < BT_UI_KATEGORIE::KATEGORIE_BT_END; i++)
	{
		m_uiKategorieButton[i]->Update();

		if (m_uiKategorieButton[i]->GetButtonIsClicked())
			m_nowSeletedType = i;
	}


	for (int i = 0; i < (i == BT_UI_KATEGORIE::SHOES ? 4 : 6); i++)
	{
		m_itemButton[m_nowSeletedType][i]->Update();

		if (m_itemButton[m_nowSeletedType][i]->GetButtonIsClicked())
			m_character->SetWearingItem(m_nowSeletedType, i);
	}

	if (m_character)	m_character->Update();

	if (m_gameShutDown) SendMessage(g_hWnd, WM_CLOSE, NULL, NULL);
}

void ScInGame::Render()
{
	Renderer->SetWorld(VECTOR2(0, 0));
	if (m_sceneBackGroundImage) Renderer->Draw(m_sceneBackGroundImage);

	if (m_character)	m_character->Render();

	for (int i = 0; i < BT_UI_MENU::MENU_BT_END; i++)
		m_uiMenuButton[i]->Render();
	m_uiKategorieButton[BT_UI_KATEGORIE::MOUTH]->Render();
	m_uiKategorieButton[BT_UI_KATEGORIE::EYES_BROW]->Render();
	m_uiKategorieButton[BT_UI_KATEGORIE::SHOES]->Render();
	m_uiKategorieButton[BT_UI_KATEGORIE::CLOTHS]->Render();
	m_uiKategorieButton[BT_UI_KATEGORIE::HAIR]->Render();

	for (int i = 0; i < (i == BT_UI_KATEGORIE::SHOES ? 4 : 6); i++)
		m_itemButton[m_nowSeletedType][i]->Render();
}

void ScInGame::Release()
{
	for (int i = 0; i < BT_UI_MENU::MENU_BT_END; i++)
	{
		m_uiMenuButton[i]->Release();
		SAFE_DELETE(m_uiMenuButton[i]);
	}
	for (int i = 0; i < BT_UI_KATEGORIE::KATEGORIE_BT_END; i++)
	{
		m_uiKategorieButton[i]->Release();
		SAFE_DELETE(m_uiKategorieButton[i]);
		for (int j = 0; j < (j == BT_UI_KATEGORIE::SHOES ? 4 : 6); j++)
		{
			m_itemButton[i][j]->Release();
			SAFE_DELETE(m_itemButton[i][j]);
		}
	}

	if (m_character)	m_character->Release();
	SAFE_DELETE(m_character);
}