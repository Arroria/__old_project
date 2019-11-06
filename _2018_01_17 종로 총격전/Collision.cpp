#include "stdafx.h"
#include "Collision.h"


Collision_box::Collision_box()
	: m_position(0, 0)
	, m_size(0, 0)
{
}
Collision_box::~Collision_box()
{
}



bool Collision_box::CalculateCollision(const Collision_box & anotherAABB)
{
	D3DXVECTOR2& anotherAABBPosition = anotherAABB.GetPosition();
	D3DXVECTOR2& anotherAABBSize = anotherAABB.GetSize();
	return
		m_position.x - m_size.x < anotherAABBPosition.x + anotherAABBSize.x &&
		m_position.y - m_size.y < anotherAABBPosition.y + anotherAABBSize.y &&
		m_position.x + m_size.x > anotherAABBPosition.x - anotherAABBSize.x &&
		m_position.y + m_size.y > anotherAABBPosition.y - anotherAABBSize.y;
}

void Collision_box::RenderBox(LPDIRECT3DDEVICE9 device)
{
	GetSingleton(PlaneRenderer)->Render(device, GetSingleton(TextureLoader)->GetTexture(L"OBB"), D3DXVECTOR3(m_position.x, m_position.y, 0), false, D3DXQUATERNION(0, 0, 0, 1), m_size * 0.25f);
}
