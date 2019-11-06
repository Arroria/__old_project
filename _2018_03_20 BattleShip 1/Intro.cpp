#include "DXUT.h"
#include "Intro.h"


Intro::Intro()
	: m_scene2(false)
	, m_noise(0)
{
	LoadTex(L"./Resource/noIngame/intro/intro1.png", L"Intro0");
	LoadTex(L"./Resource/noIngame/intro/intro0.png", L"Intro1");
	LoadTex(L"./Resource/noIngame/intro/noise0.png", L"IntroNoise0");
	LoadTex(L"./Resource/noIngame/intro/noise1.png", L"IntroNoise1");
}
Intro::~Intro()
{
}



void Intro::Initialize()
{
	m_scene2 = false;
	m_noise = 0;
}

void Intro::Update()
{
	if (SingletonInstance(Fader)->Update())
	{
		SingletonInstance(Fader)->Create(false);
		SingletonInstance(Fader)->SetStay(30);
		SingletonInstance(SceneManager)->ChangeScene(L"MainMenu");
		return;
	}
		
	if (!m_scene2)
	{
		if (SingletonInstance(InputManager)->IsKeyDown(VK_SPACE))
			m_scene2 = true;
	}
	else if (!m_noise)
	{
		if (SingletonInstance(InputManager)->IsKeyDown(VK_SPACE))
			m_noise++;
	}
	else
	{
		m_noise++;
		if (m_noise == 300)
			SingletonInstance(Fader)->Create(true);
	}
}

void Intro::Render()
{
	SPRITE->Begin(D3DXSPRITE_ALPHABLEND);

	D3DXMATRIX iden;
	D3DXMatrixIdentity(&iden);
	SPRITE->SetTransform(&iden);
	
	SPRITE->Draw(GetTex(L"Intro" + std::to_wstring(m_scene2 ? 1 : 0))->tex, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, 1));
	if (m_noise)
		SPRITE->Draw(GetTex(L"IntroNoise" + std::to_wstring(m_noise % 10 < 5))->tex, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, m_noise / 300.));


	SPRITE->End();
	SingletonInstance(Fader)->Render();
}

void Intro::Release()
{
}
