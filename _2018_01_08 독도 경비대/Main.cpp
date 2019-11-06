#include "stdafx.h"
#include "Main.h"



Main::Main()
{
}
Main::~Main()
{
}



void Main::Initialize()
{
	m_btState.assign(0);

	auto LoadTex = [](const std::wstring& path)->LPDIRECT3DTEXTURE9
	{
		LPDIRECT3DTEXTURE9 tex;
		D3DXCreateTextureFromFileExW(DEVICE, path.data(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DXCOLOR(0, 0, 0, 0), nullptr, nullptr, &tex);
		return tex;
	};
	
	m_texture.insert(std::make_pair(L"title",	LoadTex(L"./Resource/sprite/main/title.png")));
	m_texture.insert(std::make_pair(L"Credit_C",	LoadTex(L"./Resource/sprite/main/credit_ui_click.png")));
	m_texture.insert(std::make_pair(L"Credit_U",	LoadTex(L"./Resource/sprite/main/credit_ui_mouseover.png")));
	m_texture.insert(std::make_pair(L"Credit_D",	LoadTex(L"./Resource/sprite/main/credit_ui_normal.png")));
	m_texture.insert(std::make_pair(L"Howto_C",		LoadTex(L"./Resource/sprite/main/howtoplay_ui_click.png")));
	m_texture.insert(std::make_pair(L"Howto_U",		LoadTex(L"./Resource/sprite/main/howtoplay_ui_mouseover.png")));
	m_texture.insert(std::make_pair(L"Howto_D",		LoadTex(L"./Resource/sprite/main/howtoplay_ui_normal.png")));
	m_texture.insert(std::make_pair(L"Intro_C",		LoadTex(L"./Resource/sprite/main/intro_ui_click.png")));
	m_texture.insert(std::make_pair(L"Intro_U",		LoadTex(L"./Resource/sprite/main/intro_ui_mouseover.png")));
	m_texture.insert(std::make_pair(L"Intro_D",		LoadTex(L"./Resource/sprite/main/intro_ui_normal.png")));
	m_texture.insert(std::make_pair(L"Ranking_C",	LoadTex(L"./Resource/sprite/main/ranking_ui_click.png")));
	m_texture.insert(std::make_pair(L"Ranking_U",	LoadTex(L"./Resource/sprite/main/ranking_ui_mouseover.png")));
	m_texture.insert(std::make_pair(L"Ranking_D",	LoadTex(L"./Resource/sprite/main/ranking_ui_normal.png")));
	m_texture.insert(std::make_pair(L"Star_C",		LoadTex(L"./Resource/sprite/main/start_ui_click.png")));
	m_texture.insert(std::make_pair(L"Star_U",		LoadTex(L"./Resource/sprite/main/start_ui_mouseover.png")));
	m_texture.insert(std::make_pair(L"Star_D",		LoadTex(L"./Resource/sprite/main/start_ui_normal.png")));
}

void Main::Update()
{
	auto BtStateCalaulate = [](const D3DXVECTOR2& position, const D3DXVECTOR2& size, UINT& statePlace)
	{
		RECT_ex rc(
			position.x - size.x,
			position.y - size.y,
			position.x + size.x,
			position.y + size.y);

		POINT cursor = GetSingleton(InputManager)->GetMousePos();
		if (rc.left < cursor.x &&
			rc.right > cursor.x &&
			rc.top < cursor.y &&
			rc.bottom > cursor.y)
		{
			if (GetSingleton(InputManager)->IsKeyUp(VK_LBUTTON))
				statePlace = 3;
			else if (GetSingleton(InputManager)->IsKeyPressed(VK_LBUTTON))
				statePlace = 2;
			else
				statePlace = 1;
		}
		else
			statePlace = 0;
	};

	BtStateCalaulate(D3DXVECTOR2((1280) * 0.5f, 350 + (56 * 0.5f)), D3DXVECTOR2(199 * 0.5f, 56 * 0.5f), m_btState[0]);
	BtStateCalaulate(D3DXVECTOR2((1280) * 0.5f, 450 + (56 * 0.5f)), D3DXVECTOR2(199 * 0.5f, 56 * 0.5f), m_btState[1]);
	BtStateCalaulate(D3DXVECTOR2((1280) * 0.5f, 550 + (56 * 0.5f)), D3DXVECTOR2(199 * 0.5f, 56 * 0.5f), m_btState[2]);
	BtStateCalaulate(D3DXVECTOR2((1280) * 0.5f, 650 + (56 * 0.5f)), D3DXVECTOR2(199 * 0.5f, 56 * 0.5f), m_btState[3]);
	BtStateCalaulate(D3DXVECTOR2((1280) * 0.5f, 750 + (56 * 0.5f)), D3DXVECTOR2(199 * 0.5f, 56 * 0.5f), m_btState[4]);

	if (m_btState[0] == 3)
		GetSingleton(SceneManager)->ChangeScene(L"2Stage");
}

void Main::Render()
{
	LPD3DXSPRITE sp;
	D3DXCreateSprite(DEVICE, &sp);
	sp->Begin(D3DXSPRITE_ALPHABLEND);
	{
		auto Draw = [&sp](const D3DXVECTOR2& position, LPDIRECT3DTEXTURE9 texture)
		{
			D3DXMATRIX mat;
			D3DXMatrixTranslation(&mat, position.x, position.y, 0);
			sp->SetTransform(&mat);
			sp->Draw(texture, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, 1));
		};

		Draw(D3DXVECTOR2(0, 0), m_texture[L"title"]);

		Draw(D3DXVECTOR2((1280 - 199) * 0.5f, 350), m_texture[std::wstring(L"Star_") +		(!m_btState[0] ? L"D" : (m_btState[0] == 1 ? L"U" : L"C"))]);
		Draw(D3DXVECTOR2((1280 - 199) * 0.5f, 450), m_texture[std::wstring(L"Howto_") +		(!m_btState[1] ? L"D" : (m_btState[1] == 1 ? L"U" : L"C"))]);
		Draw(D3DXVECTOR2((1280 - 199) * 0.5f, 550), m_texture[std::wstring(L"Ranking_") +	(!m_btState[2] ? L"D" : (m_btState[2] == 1 ? L"U" : L"C"))]);
		Draw(D3DXVECTOR2((1280 - 199) * 0.5f, 650), m_texture[std::wstring(L"Intro_") +		(!m_btState[3] ? L"D" : (m_btState[3] == 1 ? L"U" : L"C"))]);
		Draw(D3DXVECTOR2((1280 - 199) * 0.5f, 750), m_texture[std::wstring(L"Credit_") +	(!m_btState[4] ? L"D" : (m_btState[4] == 1 ? L"U" : L"C"))]);
	}
	sp->End();
}

void Main::Release()
{
	for (auto& iter : m_texture)
		SAFE_RELEASE(iter.second)
	m_texture.clear();
}
