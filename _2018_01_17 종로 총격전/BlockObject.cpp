#include "stdafx.h"
#include "BlockObject.h"


BlockObject::BlockObject()
	: m_position(0, 0)
	, m_texture(nullptr)
{
}
BlockObject::~BlockObject()
{
}



void BlockObject::Initialize(const D3DXVECTOR2& position, const D3DXVECTOR2& collisionSize, Texture* texture)
{
	m_position = position;
	m_aabb.SetPosition(position);
	m_aabb.SetSize(collisionSize);
	m_texture = texture;
}

void BlockObject::Update()
{
}

void BlockObject::Render()
{
	if (m_texture)
		GetSingleton(PlaneRenderer)->Render(DEVICE, m_texture, D3DXVECTOR3(m_position.x, m_position.y, 0));
}

void BlockObject::Release()
{
}
