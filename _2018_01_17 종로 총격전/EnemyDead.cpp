#include "stdafx.h"
#include "EnemyDead.h"


#define AnimationFrameInterval 8
#define Duration 600

EnemyDead::EnemyDead(const D3DXVECTOR2& position, const bool& lookAtRight)
	: m_position(position)
	, m_lookAtRight(lookAtRight)

	, m_animation(0)
	, m_duration(0)
{
}
EnemyDead::~EnemyDead()
{
}


void EnemyDead::Initialize()
{
}

void EnemyDead::Update()
{
	if (m_duration >= Duration)
	{
		Expire();
		return;
	}
	m_duration++;

	if (m_animation < AnimationFrameInterval * (4 - 1))
		m_animation++;
}

void EnemyDead::Render()
{
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(std::wstring(L"EnemyDead") + std::to_wstring(m_animation / AnimationFrameInterval)), D3DXVECTOR3(m_position.x, m_position.y, 0), m_lookAtRight);
}

void EnemyDead::Release()
{
}
