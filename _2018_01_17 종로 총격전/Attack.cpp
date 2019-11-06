#include "stdafx.h"
#include "Attack.h"


Attack::Attack()
	: m_texture(nullptr)

	, m_position(0, 0)
	, m_rotation(0, 0, 0, 1)
	, m_speed(0)

	, m_duration(150)
	, m_isExpired(false)

	, m_isFake(false)
{
}
Attack::~Attack()
{
}



void Attack::Initialize(Texture* texture, const D3DXVECTOR2& position, const D3DXQUATERNION& rotation, const float& speed, const bool& isFake)
{
	m_texture = texture;

	m_position	= position;
	m_rotation	= rotation;
	m_speed		= speed;
	m_isFake	= isFake;

	m_aabb.SetSize(D3DXVECTOR2(6, 6));
	m_aabb.SetPosition(m_position);
}

void Attack::Update()
{
	D3DXVECTOR2 direction;
	D3DXMATRIX r;
	D3DXMatrixRotationQuaternion(&r, &m_rotation);
	D3DXVec2TransformCoord(&direction, &D3DXVECTOR2(0, 1), &r);

	m_position += direction * m_speed;

	if (!m_duration)
	{
		m_isExpired = true;
		return;
	}
	m_duration--;


	m_aabb.SetPosition(m_position);
}

void Attack::Render()
{
	GetSingleton(PlaneRenderer)->Render(DEVICE, m_texture, D3DXVECTOR3(m_position), false, m_rotation);
	//m_aabb.RenderBox(DEVICE);
}

void Attack::Release()
{
}
