#include "DXUT.h"
#include "Title.h"


Title::Title()
	: m_introGo(true)
	, m_frame(std::make_pair(0, 0))
{
	m_nowNew = true;
}
Title::~Title()
{
}



void Title::Initialize()
{
	m_introGo = (true);
	m_frame = (std::make_pair(0, 0));

	m_state = 1;
	if (m_nowNew)
	{
		m_state = 0;
		m_nowNew = false;
	}

	m_btnOn = -1;
	m_stateNext = 0;



	for (UINT i = 0; i < 50; i++)
	{
		TextureLoad(L"./Resource/outgame/intro/go/" + std::to_wstring(i + 1) + L".jpg", L"IntroGo" + std::to_wstring(i));
		TextureLoad(L"./Resource/outgame/intro/stop/" + std::to_wstring(i + 1) + L".jpg", L"IntroStop" + std::to_wstring(i));
	}
	
	TextureLoad(L"./Resource/outgame/title.png", L"Title");
	TextureLoad(L"./Resource/outgame/select.png", L"Main_Select");

	TextureLoad(L"./Resource/outgame/1.png",	L"Main_BtT0");
	TextureLoad(L"./Resource/outgame/4.png",	L"Main_BtT1");
	TextureLoad(L"./Resource/outgame/2.png",	L"Main_BtT2");
	TextureLoad(L"./Resource/outgame/5.png",	L"Main_BtT3");
	TextureLoad(L"./Resource/outgame/6.png",	L"Main_BtF0");
	TextureLoad(L"./Resource/outgame/9.png",	L"Main_BtF1");
	TextureLoad(L"./Resource/outgame/7.png",	L"Main_BtF2");
	TextureLoad(L"./Resource/outgame/10.png",	L"Main_BtF3");

	TextureLoad(L"./Resource/outgame/intorduce.png", L"Introduce");
	TextureLoad(L"./Resource/outgame/How_to.png", L"Howto");


}

void Title::Update()
{
	{
		constexpr UINT SPEED = 5;
		if (m_introGo)
		{
			constexpr UINT MAX = 50;

			m_frame.second++;
			if (m_frame.second >= SPEED)
			{
				m_frame.second = 0;
				m_frame.first++;
				if (m_frame.first >= MAX)
				{
					m_frame.first = 0;
					m_introGo = !m_introGo;
				}
			}
		}
		else
		{
			constexpr UINT MAX = 50;

			m_frame.second++;
			if (m_frame.second >= SPEED)
			{
				m_frame.second = 0;
				m_frame.first++;
				if (m_frame.first >= MAX)
				{
					m_frame.first = 0;
					m_introGo = !m_introGo;

					if (!m_state)
						m_state = 1;
				}
			}
		}
	}

	switch (m_state)
	{
	case 1:
	{
		auto BtnCheck = [](const D3DXVECTOR2& pos, const D3DXVECTOR2& size)
		{
			POINT mPos = SingletonInstance(InputManager)->GetMousePos();
			return
				pos.x - size.x * 0.5f < mPos.x &&
				pos.y < mPos.y &&
				mPos.x < pos.x + size.x * 0.5f &&
				mPos.y < pos.y + size.y;
		};

		m_btnOn = -1;
		if (BtnCheck(D3DXVECTOR2(800, 250 + 140 * 0), D3DXVECTOR2(280, 150))) m_btnOn = 0;
		if (BtnCheck(D3DXVECTOR2(800, 250 + 140 * 1), D3DXVECTOR2(280, 150))) m_btnOn = 1;
		if (BtnCheck(D3DXVECTOR2(800, 250 + 140 * 2), D3DXVECTOR2(280, 150))) m_btnOn = 2;
		if (BtnCheck(D3DXVECTOR2(800, 250 + 140 * 3), D3DXVECTOR2(280, 150))) m_btnOn = 3;
			
		if (m_btnOn == 0 && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)) m_stateNext = 2;
		if (m_btnOn == 1 && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)) m_stateNext = 3;
		if (m_btnOn == 2 && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)) m_stateNext = 4;
		if (m_btnOn == 3 && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)) m_stateNext = 5;
		
	}
		break;

	case 3:
	case 4:
		if (SingletonInstance(InputManager)->IsKeyDown(VK_SPACE)) m_stateNext = 1;
		break;
	}


	switch (m_stateNext)
	{
	case 1: m_state = 1;	break;
	case 2:	SingletonInstance(SceneManager)->ChangeScene(L"InGame"); break;
	case 3:	m_state = 3;	break;
	case 4:	m_state = 4;	break;
	case 5:	DXUTShutdown(); break;
	}
}

void Title::Render()
{
	D3DXMATRIX iden;
	D3DXMatrixIdentity(&iden);

	SPRITE->Begin(D3DXSPRITE_ALPHABLEND);

	float clr = (50 - m_frame.first) / 10.f;
	cout << clr << endl;
	D3DXMATRIX sm;
	D3DXMatrixScaling(&sm, 1600. / 640, 1600. / 640, 1600. / 640);
	SPRITE->SetTransform(&sm);
	SPRITE->Draw(TextureGet((m_introGo ? L"IntroGo" : L"IntroStop") + std::to_wstring(m_frame.first))->tex,
		nullptr,
		nullptr,
		&D3DXVECTOR3(0, -100, 0),
		m_introGo ? D3DXCOLOR(1, 1, 1, 1) : D3DXCOLOR(clr, clr, clr, 1));


	auto RenderSimple = [&iden](const std::wstring& key, const D3DXVECTOR2& pos)
	{
		Texture* tex = TextureGet(key);

		SPRITE->SetTransform(&iden);
		SPRITE->Draw(tex->tex,
			nullptr,
			&D3DXVECTOR3(tex->info.Width * 0.5f, 0, 0),
			&D3DXVECTOR3(pos.x, pos.y, 0),
			D3DXCOLOR(1, 1, 1, 1));
	};

	switch (m_state)
	{
	case 1:
		RenderSimple(L"Title", D3DXVECTOR2(800, 10));
		RenderSimple(L"Main_BtF0", D3DXVECTOR2(800, 250 + 140 * 0));
		RenderSimple(L"Main_BtF1", D3DXVECTOR2(800, 250 + 140 * 1));
		RenderSimple(L"Main_BtF2", D3DXVECTOR2(800, 250 + 140 * 2));
		RenderSimple(L"Main_BtF3", D3DXVECTOR2(800, 250 + 140 * 3));
		break;
	case 3:
		RenderSimple(L"Introduce", D3DXVECTOR2(800, 0));
		break;
	case 4:
		RenderSimple(L"Howto", D3DXVECTOR2(800, 0));
		break;

	}
	SPRITE->End();
}

void Title::Release()
{
}
