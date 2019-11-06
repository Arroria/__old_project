#include "DXUT.h"
#include "MainMenu.h"


enum SceneState
{
	Intro,
	Main,
	Start,
	HowTo,
	Intorduce,
	Score,
	Exit
};

MainMenu::MainMenu()
	: m_first(true)
	, m_nextState(Main)
{
}
MainMenu::~MainMenu()
{
}



bool MainMenu::BG_Update()
{
	constexpr UINT DELAY = 6;
	constexpr UINT MAX_FRAME = 49;

	m_frame.second++;
	if (m_frame.second >= DELAY)
	{
		m_frame.second = 0;
		m_frame.first++;
		if (m_frame.first ==  MAX_FRAME - 1)
			return true;

		if (m_frame.first >= MAX_FRAME)
			m_frame.first = 0;
	}
	return false;
}

void MainMenu::Initialize()
{
	if (m_first)
	{
		m_sceneState = Intro;
		m_first = false;
	}
	else
		m_sceneState = Main;

	m_select.first = -1;
	m_select.second = false;
	m_introStop = false;


	TextureLoad(L"./Resource/main/Credit.png", L"Credit");
	TextureLoad(L"./Resource/main/select.png", L"Select");
	TextureLoad(L"./Resource/main/title.png", L"Title");

	TextureLoad(L"./Resource/main/Main_1.png", L"0T");
	TextureLoad(L"./Resource/main/Main_2.png", L"1T");
	TextureLoad(L"./Resource/main/Main_3.png", L"3T");
	TextureLoad(L"./Resource/main/Main_4.png", L"2T");
	TextureLoad(L"./Resource/main/Main_5.png", L"4T");
	TextureLoad(L"./Resource/main/Main_6.png", L"0F");
	TextureLoad(L"./Resource/main/Main_7.png", L"1F");
	TextureLoad(L"./Resource/main/Main_8.png", L"3F");
	TextureLoad(L"./Resource/main/Main_9.png", L"2F");
	TextureLoad(L"./Resource/main/Main_10.png", L"4F");

	for (UINT i = 0; i < 49; i++)
		TextureLoad(L"./Resource/main/intro/" + std::to_wstring(i + (i >= 15 ? 2 : 1)) + L".jpg", L"Intro" + std::to_wstring(i));

	SingletonInstance(SoundManager)->Load(L"./Resource/snd/Main.wav", L"BGM_Main");

	m_bgm = SingletonInstance(SoundManager)->Play(L"BGM_Main");
}

void MainMenu::Update()
{
	if (SingletonInstance(Fader)->Update() && SingletonInstance(Fader)->IsIn())
	{
		SingletonInstance(Fader)->Create(false, 30);
		m_sceneState = m_nextState;
		m_frame.first = 0;
	}

	if (m_bgm && !m_bgm->IsPlaying())
		m_bgm->Play();

	switch (m_sceneState)
	{
	case Intro:
		if (!m_introStop && BG_Update())
		{
			SingletonInstance(Fader)->Create(true);
			m_introStop = true;
			m_nextState = Main;
		}
		break;

	case Main:
		BG_Update();

		{
			auto BT_On = [](const D3DXVECTOR2& focus, const D3DXVECTOR2& size)->bool
			{
				POINT mPos = SingletonInstance(InputManager)->GetMousePos();
				return
					focus.x - size.x < mPos.x &&
					focus.y - size.y < mPos.y&&
					mPos.x < focus.x + size.x &&
					mPos.y < focus.y + size.y;
			};

			m_select.first = -1;
			m_select.second = false;
			for (UINT i = 0; i < 5; i++)
			{
				if (BT_On(D3DXVECTOR2(800, i * 125 + 320), D3DXVECTOR2(120, 60)))
				{
					m_select.first = i;
					m_select.second = SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON);
					break;
				}
			}
			if (m_select.second && !SingletonInstance(Fader)->IsRun())
			{
				switch (m_select.first)
				{
				case 0:	m_nextState = Start;		SingletonInstance(Fader)->Create(true);	break;
				case 1:	m_nextState = HowTo;		SingletonInstance(Fader)->Create(true);	break;
				case 2:	m_nextState = Intorduce;	SingletonInstance(Fader)->Create(true);	break;
				case 3:	m_nextState = Score;		SingletonInstance(Fader)->Create(true);	break;
				case 4:	m_nextState = Exit;			SingletonInstance(Fader)->Create(true);	break;
				}
			}
			break;
		}

	case Start:
		if (m_bgm)
			m_bgm->Stop();
		SingletonInstance(SceneManager)->ChangeScene(L"Ingame");
		break;

	case HowTo:
	case Intorduce:
	case Score:
		if (!SingletonInstance(Fader)->IsRun() && SingletonInstance(InputManager)->IsKeyDown(VK_SPACE))
		{
			m_nextState = Main;
			SingletonInstance(Fader)->Create(true);
		}
		break;

	case Exit:
		DXUTShutdown();
		break;
	}
}

void MainMenu::Render()
{
	SPRITE->Begin(D3DXSPRITE_ALPHABLEND);


	D3DXMATRIX sm;
	D3DXMatrixScaling(&sm, 2, 2, 2);
	SPRITE->SetTransform(&sm);
	SPRITE->Draw(TextureGet(L"Intro" + std::to_wstring(m_frame.first))->tex, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, 1));


	D3DXMATRIX iden;
	D3DXMatrixIdentity(&iden);
	SPRITE->SetTransform(&iden);

	Texture * tex;
	switch (m_sceneState)
	{
	case Main:
		tex = TextureGet(L"Title");
		if (!tex)return;
		SPRITE->Draw(tex->tex, nullptr, &D3DXVECTOR3(tex->info.Width * 0.5f, 0, 0), &D3DXVECTOR3(800, 0, 0), D3DXCOLOR(1, 1, 1, 1));

		for (UINT i = 0; i < 5; i++)
		{
			tex = TextureGet(std::to_wstring(i) + (m_select.first == i ? L"T" : L"F"));
			SPRITE->Draw(tex->tex, nullptr, &D3DXVECTOR3(tex->info.Width * 0.5f, tex->info.Height * 0.5f, 0), &D3DXVECTOR3(800, i * 125 + 320, 0), D3DXCOLOR(1, 1, 1, 1));

			if (m_select.first == i)
			{
				tex = TextureGet(L"Select");
				SPRITE->Draw(tex->tex, nullptr, &D3DXVECTOR3(tex->info.Width * 0.5f, tex->info.Height * 0.5f, 0), &D3DXVECTOR3(800, i * 125 + 320, 0), D3DXCOLOR(1, 1, 1, 1));
			}
		}

		tex = TextureGet(L"Credit");
		SPRITE->Draw(tex->tex, nullptr, &D3DXVECTOR3(tex->info.Width, tex->info.Height, 0), &D3DXVECTOR3(1600 - 50, 900 - 50, 0), D3DXCOLOR(1, 1, 1, 1));
		break;

	case HowTo:
		break;

	case Intorduce:
		break;

	case Score:
		break;
	}


	SPRITE->End();
	SingletonInstance(Fader)->Render();
}

void MainMenu::Release()
{
}

