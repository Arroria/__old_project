#include "stdafx.h"
#include "Grenade.h"


Grenade::Grenade()
	: m_position(0, 0)
	, m_rotation(0, 0, 0, 1)
	, m_speed(0)
	, m_gravity(-10)

	, m_moveToLeft(false)

	, m_isExpired(false)
{
}
Grenade::~Grenade()
{
}



void Grenade::Initialize(const D3DXVECTOR2 & position, const bool & moveToLeft)
{
	m_position = position;
	m_moveToLeft = moveToLeft;

	m_aabb.SetPosition(m_position);
	m_aabb.SetSize(D3DXVECTOR2(6, 10));
}

void Grenade::Update()
{
	m_position.x -= m_moveToLeft ? 5 : -5;
	m_aabb.SetPosition(m_position);
	m_gravity += 0.5f;
	m_position.y -= m_gravity;
}

void Grenade::Render()
{
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"Grenade"), D3DXVECTOR3(m_position.x, m_position.y, 0));
}

void Grenade::Release()
{
}
