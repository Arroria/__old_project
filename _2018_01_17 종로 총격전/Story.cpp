#include "stdafx.h"
#include "Story.h"


Story::Story()
{
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Story", L"./Resource/scene/stageStory.png");
}
Story::~Story()
{
}



void Story::Initialize()
{
	m_scale = 900.f / 508.f;
	m_position = 1934 * 0.5f * m_scale;

	m_counter = 0;
}

void Story::Update()
{
	if (GetSingleton(InputManager)->IsKeyDown(VK_RETURN))
	{
		GetSingleton(SceneManager)->ChangeScene(L"Stage2");
		return;
	}


	if (m_counter)
	{
		if (m_counter > 90 || GetSingleton(InputManager)->IsKeyDown(VK_SPACE))
			GetSingleton(SceneManager)->ChangeScene(L"Stage2");
		m_counter++;
	}

	m_position -= (GetSingleton(InputManager)->IsKeyPressed(VK_SPACE) ? 8 : 3);
	if (1934 * -0.5f * m_scale + 1600 > m_position)
	{
		m_position = 1934 * -0.5f * m_scale + 1600;
		m_counter++;
	}
}

void Story::Render()
{
	GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(L"Story"), D3DXVECTOR3(m_position, 450, 0), D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(1, 1) * m_scale);
}

void Story::Release()
{
}
