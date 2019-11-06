#include "DXUT.h"
#include "GameUI.h"


GameUI::GameUI(const bool & forPlayer)
	: m_forPlayer(forPlayer)
{
}
GameUI::~GameUI()
{
}



void GameUI::Update()
{
	constexpr UINT SPEED = 4;
	constexpr UINT MAX = 3;

	m_frame.second++;
	if (m_frame.second >= SPEED)
	{
		m_frame.second = 0;
		m_frame.first++;
		if (m_frame.first >= MAX)
		{
			m_frame.first = 0;
		}
	}
}

void GameUI::Render(const std::wstring & key, const D3DXVECTOR2& pos, const bool& darkness, const bool & isMiddel)
{
	Texture* tex = TextureGet(key);
	D3DXVECTOR3 pivot = D3DXVECTOR3(
		isMiddel ? tex->info.Width * 0.5f : 0,
		isMiddel ? tex->info.Height * 0.5f : 0,
		0);
	D3DXMATRIX tm;
	D3DXMatrixTranslation(&tm, pos.x, pos.y, 0);

	SPRITE->SetTransform(&tm);
	SPRITE->Draw(tex->tex, nullptr, &pivot, nullptr, darkness ? D3DXCOLOR(0.5, 0.5, 0.5, 1) : D3DXCOLOR(1, 1, 1, 1));
}



void GameUI::R_Frame(const D3DXVECTOR2 & pos)
{
	Render(GetPE() + L"Frame", pos, false);
}

void GameUI::R_Item(const D3DXVECTOR2 & pos, const UINT & index, const bool & shiny)
{
	Render(GetPE() + L"Item" + std::to_wstring(index), pos, !shiny, true);
}

void GameUI::R_Tool(const D3DXVECTOR2 & pos, const UINT & index, const bool & shiny)
{
	switch (index)
	{
	case 0:	Render(std::wstring(L"RandomPlace")	, pos, shiny, true);	break;
	case 1:	Render(std::wstring(L"Reset")		, pos, shiny, true);	break;
	case 2:	Render(std::wstring(L"Start")		, pos, shiny, true);	break;
	}
}

void GameUI::R_ToolBox( const UINT & index)
{
	POINT mousePos = SingletonInstance(InputManager)->GetMousePos();

	switch (index)
	{
	case 0:	Render(std::wstring(L"RandomPlaceText"),	D3DXVECTOR2(mousePos.x, mousePos.y), false);	break;
	case 1:	Render(std::wstring(L"ResetText"),			D3DXVECTOR2(mousePos.x, mousePos.y), false);	break;
	case 2:	Render(std::wstring(L"StartText"),			D3DXVECTOR2(mousePos.x, mousePos.y), false);	break;
	case 3:	Render(std::wstring(L"SkillCancelText"),	D3DXVECTOR2(mousePos.x, mousePos.y), false);	break;
	}
}

void GameUI::R_Ship(const D3DXVECTOR2 & pos, const UINT & size, const bool& shiny, const bool & noise)
{
	if (noise)
		Render(L"Noise" + std::to_wstring(size) + std::to_wstring(m_frame.first), pos, false);
	else
		Render(GetPE() + L"ShipX" + std::to_wstring(size), pos, !shiny, false);
}
