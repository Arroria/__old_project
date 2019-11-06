#include "DXUT.h"
#include "GameUI.h"


GameUI::GameUI(const bool & isLeft)
	: m_isLeft(isLeft)
{
	TextureLoad(L"./Resource/ui/player/frame.png", L"pFrame");
	TextureLoad(L"./Resource/ui/enemy/frame.png", L"eFrame");

	for (UINT i = 2; i < 5; i++)
	{
		TextureLoad(L"./Resource/ui/player/x" + std::to_wstring(i) + L".png", L"px" + std::to_wstring(i));
		TextureLoad(L"./Resource/ui/enemy/x" + std::to_wstring(i) + L".png", L"ex" + std::to_wstring(i));
		for (UINT j = 0; j < 3; j++)
			TextureLoad(L"./Resource/ui/x" + std::to_wstring(i) + L"Noise" + std::to_wstring(j) + L".png", L"x" + std::to_wstring(i) + L"Noise" + std::to_wstring(j));
	}

	for (UINT i = 0; i < 4; i++)
	{
		TextureLoad(L"./Resource/ui/player/item" + std::to_wstring(i) + L"F.png", L"pItem" + std::to_wstring(i) + L"F");
		TextureLoad(L"./Resource/ui/player/item" + std::to_wstring(i) + L"T.png", L"pItem" + std::to_wstring(i) + L"T");
		TextureLoad(L"./Resource/ui/enemy/item" + std::to_wstring(i) + L"F.png", L"eItem" + std::to_wstring(i) + L"F");
		TextureLoad(L"./Resource/ui/enemy/item" + std::to_wstring(i) + L"T.png", L"eItem" + std::to_wstring(i) + L"T");
	}

	TextureLoad(L"./Resource/ui/player/resetT.png",				L"ResetT");
	TextureLoad(L"./Resource/ui/player/resetF.png",				L"ResetF");
	TextureLoad(L"./Resource/ui/player/resetText.png",			L"ResetW");
	TextureLoad(L"./Resource/ui/player/randomPlaceT.png",		L"RandomPlaceT");
	TextureLoad(L"./Resource/ui/player/randomPlaceF.png",		L"RandomPlaceF");
	TextureLoad(L"./Resource/ui/player/randomPlaceText.png",	L"RandomPlaceW");
	TextureLoad(L"./Resource/ui/player/startT.png",				L"StartT");
	TextureLoad(L"./Resource/ui/player/startF.png",				L"StartF");
	TextureLoad(L"./Resource/ui/player/startText.png",			L"StartW");
}
GameUI::~GameUI()
{
}



void GameUI::Update()
{
	constexpr UINT FRAME_SPEED = 4;
	constexpr UINT FRAME_LIMITE = 3;

	m_frame.second++;
	if (m_frame.second >= FRAME_SPEED)
	{
		m_frame.second = 0;
		m_frame.first++;
		if (m_frame.first >= FRAME_LIMITE)
			m_frame.first = 0;
	}
}

void GameUI::RenderBegin()
{
	SPRITE->Begin(D3DXSPRITE_ALPHABLEND);
	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);
	SPRITE->SetTransform(&identity);
}

void GameUI::RenderBase(const D3DXVECTOR2 & pos)
{
	Texture* tex = TextureGet(P_E_Check() + L"Frame");
	SPRITE->Draw(tex->tex, nullptr, &D3DXVECTOR3(m_isLeft ? 0 : tex->info.Width, 0, 0), &D3DXVECTOR3(pos.x, pos.y, 0), D3DXCOLOR(1, 1, 1, 1));
}

void GameUI::RenderShip(const D3DXVECTOR2 & pos, const UINT& index, const bool & isExpired, const bool& isDark)
{
	Texture* tex = TextureGet(isExpired ?
		L"x" + std::to_wstring(index) + L"Noise" + std::to_wstring(m_frame.first) :
		P_E_Check() + L"x" + std::to_wstring(index));
	SPRITE->Draw(tex->tex, nullptr, &D3DXVECTOR3(m_isLeft ? 0 : tex->info.Width, 0, 0), &D3DXVECTOR3(pos.x, pos.y, 0), D3DXCOLOR(isDark ? 0.5f : 1, isDark ? 0.5f : 1, isDark ? 0.5f : 1, 1));
}

void GameUI::RenderItem(const D3DXVECTOR2 & pos, const UINT& index, const bool & isGlow)
{
	Texture* tex = TextureGet(P_E_Check() + L"Item" + std::to_wstring(index) + (isGlow ? L"T" : L"F"));
	SPRITE->Draw(tex->tex, nullptr, &D3DXVECTOR3(tex->info.Width * 0.5, tex->info.Height * 0.5, 0), &D3DXVECTOR3(pos.x, pos.y, 0), D3DXCOLOR(1, 1, 1, 1));
}

void GameUI::RenderBtn(const D3DXVECTOR2 & pos, const UINT& index, const bool & isGlow)
{
	std::wstring key;
	switch (index)
	{
	case 0:	key += L"Reset";	break;
	case 1:	key += L"RandomPlace";	break;
	case 2:	key += L"Start";	break;
	}
	
	SPRITE->Draw(TextureGet(key + (isGlow ? L"T" : L"F"))->tex, nullptr, nullptr, &D3DXVECTOR3(pos.x, pos.y, 0), D3DXCOLOR(1, 1, 1, 1));
}

void GameUI::RenderBtnInfo(const UINT & index)
{
	std::wstring key;
	switch (index)
	{
	case 0:	key += L"Reset";	break;
	case 1:	key += L"RandomPlace";	break;
	case 2:	key += L"Start";	break;
	}
	POINT mousePos = SingletonInstance(InputManager)->GetMousePos();
	SPRITE->Draw(TextureGet(key + L"W")->tex, nullptr, nullptr, &D3DXVECTOR3(mousePos.x, mousePos.y, 0), D3DXCOLOR(1, 1, 1, 1));
}
