#include "stdafx.h"
#include "Intro.h"


Intro::Intro()
	: m_position(0)
	, m_scale(0)

	, m_counter(0)
{
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Intro", L"./Resource/scene/intro.png");
}
Intro::~Intro()
{
}



void Intro::Initialize()
{
	m_scale = 900.f / 508.f;
	m_position = 2950 * 0.5f * m_scale;

	m_counter = 0;
}

void Intro::Update()
{
	if (GetSingleton(InputManager)->IsKeyDown(VK_RETURN))
	{
		GetSingleton(SceneManager)->ChangeScene(L"Title");
		return;
	}


	if (m_counter)
	{
		if (m_counter > 90 || GetSingleton(InputManager)->IsKeyDown(VK_SPACE))
			GetSingleton(SceneManager)->ChangeScene(L"Title");
		m_counter++;
	}

	m_position -= (GetSingleton(InputManager)->IsKeyPressed(VK_SPACE) ? 8 : 3);
	if (2950 * -0.5f * m_scale + 1600 > m_position)
	{
		m_position = 2950 * -0.5f * m_scale + 1600;
		m_counter++;
	}
}

void Intro::Render()
{
	GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(L"Intro"), D3DXVECTOR3(m_position, 450, 0), D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(1, 1) * m_scale);
}

void Intro::Release()
{
}
