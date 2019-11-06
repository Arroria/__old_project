#include "stdafx.h"
#include "Tilte.h"


Tilte::Tilte()
{
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Title", L"./Resource/scene/title/main.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"BT_Credit_C",	L"./Resource/scene/title/credit_ui_click.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"BT_Credit_N",	L"./Resource/scene/title/credit_ui_mouseover.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"BT_Credit_O",	L"./Resource/scene/title/credit_ui_normal.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"BT_Start_C",		L"./Resource/scene/title/gamestart_ui_click.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"BT_Start_N",		L"./Resource/scene/title/gamestart_ui_mouseover.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"BT_Start_O",		L"./Resource/scene/title/gamestart_ui_normal.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"BT_HowTo_C",		L"./Resource/scene/title/howtoplay_ui_click.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"BT_HowTo_N",		L"./Resource/scene/title/howtoplay_ui_mouseover.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"BT_HowTo_O",		L"./Resource/scene/title/howtoplay_ui_normal.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"BT_Ranking_C",	L"./Resource/scene/title/ranking_ui_click.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"BT_Ranking_N",	L"./Resource/scene/title/ranking_ui_mouseover.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"BT_Ranking_O",	L"./Resource/scene/title/ranking_ui_normal.png");
	
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Credit",		L"./Resource/scene/credit.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"HowTo",		L"./Resource/scene/howtoplay.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Ranking",	L"./Resource/scene/ranking.png");
}
Tilte::~Tilte()
{
}



void Tilte::Initialize()
{
	m_btState.fill(0);
	m_overScene.clear();

	g_score = 0;
}

void Tilte::Update()
{
	if (m_overScene.empty())
	{
		auto MouseCrash = [](const D3DXVECTOR2& position, const D3DXVECTOR2& size, const POINT& mousePosition)->bool
		{
			return
				position.x - size.x < mousePosition.x &&
				position.x + size.x > mousePosition.x &&
				position.y - size.y < mousePosition.y &&
				position.y + size.y > mousePosition.y;
		};

		for (UINT index = 0; index < 4; index++)
		{
			if (MouseCrash(D3DXVECTOR2(800, 450 + 60 * index), D3DXVECTOR2(88, 18.5f), GetSingleton(InputManager)->GetMousePos()))
			{
				if (GetSingleton(InputManager)->IsKeyUp(VK_LBUTTON))
				{
					switch (index)
					{
					case 0:
						GetSingleton(SceneManager)->ChangeScene(L"Stage1");
						return;
					case 1:	m_overScene = L"HowTo";		break;
					case 2:	m_overScene = L"Ranking";	break;
					case 3:	m_overScene = L"Credit";	break;
					}
				}
				else
					m_btState[index] = GetSingleton(InputManager)->IsKeyPressed(VK_LBUTTON) ? 2 : 1;
			}
			else
				m_btState[index] = 0;
		}
	}
	else if (GetSingleton(InputManager)->IsKeyDown(VK_LBUTTON))
		m_overScene.clear();
}

void Tilte::Render()
{
	GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(L"Title"), D3DXVECTOR3(800, 450, 0));
	GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(std::wstring(L"BT_Start_")	+ (m_btState[0] == 0 ? L"N" : (m_btState[0] == 1 ? L"O" : L"C"))), D3DXVECTOR3(800, 450 + 60 * 0, 0));
	GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(std::wstring(L"BT_HowTo_")	+ (m_btState[1] == 0 ? L"N" : (m_btState[1] == 1 ? L"O" : L"C"))), D3DXVECTOR3(800, 450 + 60 * 1, 0));
	GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(std::wstring(L"BT_Ranking_")	+ (m_btState[2] == 0 ? L"N" : (m_btState[2] == 1 ? L"O" : L"C"))), D3DXVECTOR3(800, 450 + 60 * 2, 0));
	GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(std::wstring(L"BT_Credit_")	+ (m_btState[3] == 0 ? L"N" : (m_btState[3] == 1 ? L"O" : L"C"))), D3DXVECTOR3(800, 450 + 60 * 3, 0));
	
	if (!m_overScene.empty())
		GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(m_overScene), D3DXVECTOR3(800, 450, 0));
}

void Tilte::Release()
{
}
