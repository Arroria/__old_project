#include "stdafx.h"
#include "Ending.h"


Ending::Ending()
	: m_position(0)
	, m_scale(0)

	, m_counter(0)
{
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Ending", L"./Resource/scene/ending.png");
}
Ending::~Ending()
{
}



void Ending::Initialize()
{
	m_scale = 900.f / 508.f;
	m_position = 2036 * 0.5f * m_scale;

	m_counter = 0;
}

void Ending::Update()
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
	if (2036 * -0.5f * m_scale + 1600 > m_position)
	{
		m_position = 2036 * -0.5f * m_scale + 1600;
		m_counter++;
	}
}

void Ending::Render()
{
	GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(L"Ending"), D3DXVECTOR3(m_position, 450, 0), D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(1, 1) * m_scale);
}

void Ending::Release()
{
}

