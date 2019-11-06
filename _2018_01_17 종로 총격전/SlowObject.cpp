#include "stdafx.h"
#include "SlowObject.h"


SlowObject::SlowObject()
	: m_position(0, 0)
	, m_tex(nullptr)

	, m_health(10)
{
}
SlowObject::~SlowObject()
{
}



void SlowObject::Initialize(const D3DXVECTOR2& position)
{
	m_position = position;
	m_aabb.SetPosition(position);
	m_aabb.SetSize(D3DXVECTOR2(45, 47) * 0.5f);

	m_tex = GetSingleton(TextureLoader)->GetTexture(rand() % 2 ? L"Object_Tile" : L"Object_Cat");
}

void SlowObject::Update()
{
}

void SlowObject::Render()
{
	GetSingleton(PlaneRenderer)->Render(DEVICE, m_tex, D3DXVECTOR3(m_position.x, m_position.y, 0));
}

void SlowObject::Release()
{
}
