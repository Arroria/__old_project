#include "DXUT.h"
#include "GameUI.h"

#include "Ship.h"


GameUI::GameUI(const bool& isRed)
	: m_isRed(isRed)
	, m_frame(0)
{
	m_shipState.fill(0);
	m_skillState.fill(true);


	LoadTex(L"./Resource/ui/3.png",	L"ico_x2");
	LoadTex(L"./Resource/ui/2.png",	L"ico_x3");
	LoadTex(L"./Resource/ui/4.png",	L"ico_x4");
	LoadTex(L"./Resource/ui/2r.png", L"Rico_x2");
	LoadTex(L"./Resource/ui/3r.png", L"Rico_x3");
	LoadTex(L"./Resource/ui/4r.png", L"Rico_x4");
	LoadTex(L"./Resource/ui/Shield.png",			L"ico_item0");
	LoadTex(L"./Resource/ui/Missile.png",			L"ico_item1");
	LoadTex(L"./Resource/ui/Scan.png",				L"ico_item2");
	LoadTex(L"./Resource/ui/Missile_Selected.png",	L"ico_item3");
	LoadTex(L"./Resource/ui/Shieldr.png",			L"Rico_item0");
	LoadTex(L"./Resource/ui/Missiler.png",			L"Rico_item1");
	LoadTex(L"./Resource/ui/Scanr.png",				L"Rico_item2");
	LoadTex(L"./Resource/ui/Missile_Selectedr.png",	L"Rico_item3");
	LoadTex(L"./Resource/ui/Noise_0.png",	L"noise0");
	LoadTex(L"./Resource/ui/Noise_1.png",	L"noise1");
	LoadTex(L"./Resource/ui/Noise_2.png",	L"noise2");
	LoadTex(L"./Resource/ui/Noise_3.png",	L"noise3");
	LoadTex(L"./Resource/ui/Main_bar.png",	L"frame");
	LoadTex(L"./Resource/ui/Main_barr.png",	L"Rframe");
	LoadTex(L"./Resource/ui/selete.png",	L"selete");

	for (UINT i = 0; i < 4; i++)
		LoadTex(L"./Resource/ui/Noise_" + std::to_wstring(i) + L".png", L"Noise" + std::to_wstring(i));
}
GameUI::~GameUI()
{
}



void GameUI::Update()
{
	m_frame++;
	if (m_frame >= 4 * 8)
	{
		m_frame = 0;
	}
}

void GameUI::Render(const std::array<Ship*, 5>& shipList, const D3DXVECTOR2& pos, const bool& isPlaying, const UINT& selectCursorPos)
{
	auto GetColorTex = [this](const std::wstring& key)->Texture*
	{
		if (m_isRed)
			return GetTex( L"R" + key );
		return GetTex( key );
	};


	SPRITE->Begin(D3DXSPRITE_ALPHABLEND);
	
	D3DXMATRIX worldTM;
	D3DXMatrixTranslation(&worldTM, pos.x, pos.y, 0);

	auto DrawSprite = [](Texture* texture, const D3DXMATRIX& worldM, const D3DXCOLOR& color = D3DXCOLOR(1, 1, 1, 1))
	{
		SPRITE->SetTransform(&worldM);
		SPRITE->Draw(texture->tex, nullptr, nullptr, 0, color);
	};

	/* Frame */ DrawSprite( GetColorTex(L"frame"), worldTM);
	/* Ship ico */ {
		
		auto Drawer = [&, this](const UINT& index, const D3DXVECTOR2& pos)
		{
			Texture* tex = shipList[index]->GetLife() ?
				GetColorTex(L"ico_x" + std::to_wstring(shipList[index]->GetSize())) :
				GetTex(L"Noise" + std::to_wstring(m_frame / 8));

			D3DXMATRIX tm;
			D3DXMatrixTranslation(&tm, pos.x, pos.y, 0);
			DrawSprite(tex,
				tm * worldTM,
				(m_shipState[index] == 1 && !isPlaying) ? D3DXCOLOR(0.5f, 0.5f, 0.5f, 1) : D3DXCOLOR(1, 1, 1, 1));
		};
		Drawer(0, D3DXVECTOR2(25		+ (m_isRed ? 36 : 0)	, 21));
		Drawer(1, D3DXVECTOR2(25 + 211	+ (m_isRed ? 36 : 0)	, 21));
		Drawer(2, D3DXVECTOR2(25		+ (m_isRed ? 36 : 0)	, 21 + 110));
		Drawer(3, D3DXVECTOR2(25 + 211	+ (m_isRed ? 36 : 0)	, 21 + 110));
		Drawer(4, D3DXVECTOR2(25 +	(m_isRed ? 211 + 36 : 0)	, 21 + 220));


	}
	/* Item ico */
	for (UINT i = 0; i < 2; i++)
	{
		for (UINT j = 0; j < 2; j++)
		{
			Texture* tex = GetColorTex(L"ico_item" + std::to_wstring(i + j * 2));
			D3DXMATRIX tm;
			D3DXMatrixTranslation(&tm, 22 + i * 105 + (m_isRed ? 247 : 0), 377 + j * 105, 0);
			DrawSprite(tex, tm * worldTM);
		}
	}

	/* Cursor */ {
		if (!isPlaying && selectCursorPos != -1)
		{
			auto い畳蟹嬢でけ葛ば嬢びたけ敢び嬢とい溝たでし = [&]()->D3DXVECTOR2
			{

			switch (selectCursorPos)
			{
				case 0:	return	D3DXVECTOR2(25							, 21);
				case 1:	return	D3DXVECTOR2(25 + 211					, 21);
				case 2:	return	D3DXVECTOR2(25							, 21 + 110);
				case 3:	return	D3DXVECTOR2(25 + 211					, 21 + 110);
				case 4:	return	D3DXVECTOR2(25 + (m_isRed ? 211 : 0)	, 21 + 220);
			}
			};

			D3DXVECTOR2 posss = い畳蟹嬢でけ葛ば嬢びたけ敢び嬢とい溝たでし();
			D3DXMATRIX tm;
			D3DXMatrixTranslation(&tm, posss.x - 13, posss.y - 13, 0);
			DrawSprite(GetColorTex(L"selete"), tm * worldTM);
		}
	}

	SPRITE->End();
}



void GameUI::ShipSetted(const UINT & index)		{ m_shipState[index] = 1; }
void GameUI::ShipDestroyed(const UINT & index)	{ m_shipState[index] = 2; }
void GameUI::SkillUsed(const UINT & index)		{ m_skillState[index] = false; }