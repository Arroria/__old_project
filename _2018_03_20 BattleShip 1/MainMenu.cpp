#include "DXUT.h"
#include "MainMenu.h"


enum TitleState
{
	Start,
	Main,
	Howto,
	Ranking,
	Credit,
	Exit
};


MainMenu::MainMenu()
	: m_state(TitleState::Main)
	, m_nextState(TitleState::Main)

	, m_runAnime(false)
	, m_mainFrame(0)
	, m_mainFrameOST(0)

	, m_btnFrame(0)

	, m_logoAnime(0)
	, m_logoOn(false)

	, m_channel(nullptr)
{
	LoadTex(L"./Resource/noIngame/main/Logo_Off.png",	L"Logo_F");
	LoadTex(L"./Resource/noIngame/main/Logo_On.png",	L"Logo_T");


	LoadTex(L"./Resource/noIngame/main/Credit_Off.png",	L"Credit_F");
	LoadTex(L"./Resource/noIngame/main/Credit_On.png",	L"Credit_T");
	LoadTex(L"./Resource/noIngame/main/Howto_Off.png",	L"HowTo_F");
	LoadTex(L"./Resource/noIngame/main/Howto_On.png",	L"HowTo_T");
	LoadTex(L"./Resource/noIngame/main/Start_Off.png",	L"Start_F");
	LoadTex(L"./Resource/noIngame/main/Start_On.png",	L"Start_T");
	LoadTex(L"./Resource/noIngame/main/Exit_Off.png",	L"Exit_F");
	LoadTex(L"./Resource/noIngame/main/Exit_On.png",	L"Exit_T");
	LoadTex(L"./Resource/noIngame/main/Ranking_Off.png",L"Ranking_F");
	LoadTex(L"./Resource/noIngame/main/Ranking_On.png",	L"Ranking_T");

	LoadTex(L"./Resource/noIngame/Credit.png", L"Credit");
	LoadTex(L"./Resource/noIngame/How_to.png", L"HowTo");
	LoadTex(L"./Resource/noIngame/Score.png", L"Ranking");
	LoadTex(L"./Resource/noIngame/Loading_0.png", L"LoadingL");
	LoadTex(L"./Resource/noIngame/Loading_1.png", L"LoadingR");

	for (UINT i = 0; i < 6; i++)
		LoadTex(L"./Resource/noIngame/Butten Anmation_" + std::to_wstring(i) + L".png", L"BtnAnime" + std::to_wstring(i));

	for (UINT i = 0; i < 20; i++)
		LoadTex(L"./Resource/noIngame/titleIntro/Main_" + std::to_wstring(i) + L".png", L"TitleAnime_Run" + std::to_wstring(i));
	for (UINT i = 0; i < 9; i++)
		LoadTex(L"./Resource/noIngame/titleIntro/Main_2_" + std::to_wstring(i + 1) + L".png", L"TitleAnime_Stay" + std::to_wstring(i));

}
MainMenu::~MainMenu()
{
}



void MainMenu::Initialize()
{
	m_channel = SingletonInstance(SoundManager)->Play(L"BGM_Main");


	m_state = TitleState::Main;
	m_nextState = TitleState::Main;

	m_runAnime = true;
	m_mainFrame = 0;
	m_mainFrameOST = 0;
	m_btnFrame = 0;
	m_logoAnime = 0;
	m_logoOn = false;


	D3DXCreateFontW(DEVICE, 60, 0, FW_DONTCARE, 1, false, DEFAULT_CHARSET, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"HYShortSamul-Medium", &m_scoreFont);
}

void MainMenu::Update()
{
	if (!m_channel->IsPlaying())
		m_channel->Play();

	if (SingletonInstance(Fader)->Update() && SingletonInstance(Fader)->IsIn())
	{
		SingletonInstance(Fader)->Create(false);
		SingletonInstance(Fader)->SetStay(30);

		if (m_nextState == TitleState::Start)
		{
			SingletonInstance(SceneManager)->ChangeScene(L"InGame");
			return;
		}
		else if (m_nextState == TitleState::Exit)
		{
			DXUTShutdown(0);
			return;
		}
		m_state = m_nextState;
	}

	switch (m_state)
	{
	case Main:
	{
		/* Anime Frame */ {
			if (!SingletonInstance(Fader)->IsRunning())
			{
				m_mainFrameOST++;
				if (m_mainFrameOST >= 6)
				{
					m_mainFrameOST = 0;
					m_mainFrame++;
					if (m_runAnime)
					{
						if (m_mainFrame >= 20)
						{
							m_mainFrame = 0;
							m_runAnime = false;
						}
					}
					else
					{
						if (m_mainFrame >= 9)
							m_mainFrame = 0;
					}
				}
			}

			m_btnFrame++;
			if (m_btnFrame == 2)
				SingletonInstance(SoundManager)->Play(L"Btn");


			if (m_logoAnime)
				m_logoAnime--;
			else
			{
				m_logoAnime = 8;
				m_logoOn = rand() & 0b1;
			}
		}



		auto MouseIn = [](const D3DXVECTOR2 &lt, const D3DXVECTOR2 &rb)->bool
		{
			POINT mPos = SingletonInstance(InputManager)->GetMousePos();
			return lt.x < mPos.x &&
				lt.y < mPos.y &&
				mPos.x < rb.x &&
				mPos.y < rb.y;
		};

		bool mouseInBtn[5];
		mouseInBtn[0] =	MouseIn(D3DXVECTOR2(16	, 240		), D3DXVECTOR2(16 + 350		, 240 + 140			));
		mouseInBtn[1] =	MouseIn(D3DXVECTOR2(343	, 240 + 115	), D3DXVECTOR2(343 + 350	, 240 + 115 + 140	));
		mouseInBtn[2] =	MouseIn(D3DXVECTOR2(16	, 240 + 230	), D3DXVECTOR2(16 + 350		, 240 + 230 + 140	));
		mouseInBtn[3] =	MouseIn(D3DXVECTOR2(343	, 240 + 345	), D3DXVECTOR2(343 + 350	, 240 + 345 + 140	));
		mouseInBtn[4] =	MouseIn(D3DXVECTOR2(16	, 240 + 460	), D3DXVECTOR2(16 + 350		, 240 + 460 + 140	));

		if (mouseInBtn[0] && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON))	{ m_nextState = TitleState::Start;		SingletonInstance(Fader)->Create(true); }
		if (mouseInBtn[1] && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON))	{ m_nextState = TitleState::Howto;		SingletonInstance(Fader)->Create(true); }
		if (mouseInBtn[2] && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON))	{ m_nextState = TitleState::Ranking;	SingletonInstance(Fader)->Create(true); }
		if (mouseInBtn[3] && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON))	{ m_nextState = TitleState::Credit;		SingletonInstance(Fader)->Create(true); }
		if (mouseInBtn[4] && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON))	{ m_nextState = TitleState::Exit;		SingletonInstance(Fader)->Create(true); }

		if (!(mouseInBtn[0] ||
			mouseInBtn[1] ||
			mouseInBtn[2] ||
			mouseInBtn[3] ||
			mouseInBtn[4]))
			m_btnFrame = 0;

		break;
	}


	case Ranking:
	case Howto:
	case Credit:
		if (SingletonInstance(InputManager)->IsKeyDown(VK_SPACE))
		{
			m_runAnime = true;
			m_mainFrame = 0;
			m_mainFrameOST = 0;

			m_nextState = TitleState::Main;
			SingletonInstance(Fader)->Create(true);
		}
		break;
	}
}

void MainMenu::Render()
{
	SPRITE->Begin(D3DXSPRITE_ALPHABLEND);
	D3DXMATRIX iden;
	D3DXMatrixIdentity(&iden);
	SPRITE->SetTransform(&iden);
	switch (m_state)
	{
	case Main:
	{
		m_runAnime ?
			SPRITE->Draw(GetTex(L"TitleAnime_Run" + std::to_wstring(m_mainFrame))->tex, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, 1)) :
			SPRITE->Draw(GetTex(L"TitleAnime_Stay" + std::to_wstring(m_mainFrame))->tex, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, 1));

		auto MouseIn = [](const D3DXVECTOR2 &lt, const D3DXVECTOR2 &rb)->bool
		{
			POINT mPos = SingletonInstance(InputManager)->GetMousePos();
			return lt.x < mPos.x &&
				lt.y < mPos.y &&
				mPos.x < rb.x &&
				mPos.y < rb.y;
		};
		auto RenderBtn = [this](const std::wstring& key, const bool& isMouseIn, const D3DXVECTOR2& pos)
		{
			D3DXMATRIX tm;
			D3DXMatrixTranslation(&tm, pos.x, pos.y, 0);
			SPRITE->SetTransform(&tm);
			SPRITE->Draw(GetTex(key + (isMouseIn ? L"_T" : L"_F"))->tex, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, 1));

			if (isMouseIn && m_btnFrame < 4 * 2)
			{
				D3DXMATRIX tm;
				D3DXMatrixTranslation(&tm, pos.x - 30, pos.y - 45, 0);
				SPRITE->SetTransform(&tm);
				SPRITE->Draw(GetTex(L"BtnAnime" + std::to_wstring(m_btnFrame / 2))->tex, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, 1));
			}
		};

		RenderBtn(L"Start"		, MouseIn(D3DXVECTOR2(16	, 240		), D3DXVECTOR2(16 + 350		, 240 + 140			)), D3DXVECTOR2(16	, 240		));
		RenderBtn(L"HowTo"		, MouseIn(D3DXVECTOR2(343	, 240 + 115	), D3DXVECTOR2(343 + 350	, 240 + 115 + 140	)), D3DXVECTOR2(343	, 240 + 115	));
		RenderBtn(L"Ranking"	, MouseIn(D3DXVECTOR2(16	, 240 + 230	), D3DXVECTOR2(16 + 350		, 240 + 230 + 140	)), D3DXVECTOR2(16	, 240 + 230	));
		RenderBtn(L"Credit"		, MouseIn(D3DXVECTOR2(343	, 240 + 345	), D3DXVECTOR2(343 + 350	, 240 + 345 + 140	)), D3DXVECTOR2(343	, 240 + 345	));
		RenderBtn(L"Exit"		, MouseIn(D3DXVECTOR2(16	, 240 + 460	), D3DXVECTOR2(16 + 350		, 240 + 460 + 140	)), D3DXVECTOR2(16	, 240 + 460	));

		/* Logo */ {
			D3DXMATRIX tm;
			D3DXMatrixTranslation(&tm, 0, 0, 0);
			SPRITE->SetTransform(&tm);
			SPRITE->Draw(GetTex(std::wstring(L"Logo") + (m_logoOn ? L"_T" : L"_F"))->tex, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, 1));
		}
		break;
	}


	case Howto:
		SPRITE->Draw(GetTex(L"HowTo")->tex, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, 1));
		break;


	case Ranking:
	{
		SPRITE->Draw(GetTex(L"Ranking")->tex, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, 1));

		D3DXMATRIX m;
		D3DXMatrixIdentity(&m);
		SPRITE->SetTransform(&m);

		RECT rc;
		SetRect(&rc, 405, 546, 405, 546);
		m_scoreFont->DrawTextW(SPRITE, (std::to_wstring(SingletonInstance(Score)->GetScore()) + L" Á¡").data(), -1, &rc, DT_NOCLIP | DT_VCENTER | DT_RIGHT, D3DXCOLOR(41 / 255., 235 / 255., 1, 1));
		SetRect(&rc, 1323, 546, 1323, 546);
		m_scoreFont->DrawTextW(SPRITE, (std::to_wstring(SingletonInstance(Score)->GetHighScore()) + L" Á¡").data(), -1, &rc, DT_NOCLIP | DT_VCENTER | DT_RIGHT, D3DXCOLOR(41 / 255., 235 / 255., 1, 1));
		break;
	}


	case Credit:
		SPRITE->Draw(GetTex(L"Credit")->tex, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, 1));
		break;
	}
	SPRITE->End();


	SingletonInstance(Fader)->Render();
}

void MainMenu::Release()
{
	m_channel->Stop();
	SAFE_RELEASE( m_scoreFont );
}
