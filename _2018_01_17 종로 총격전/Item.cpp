#include "stdafx.h"
#include "Item.h"


Item::Item()
	: m_position(0, 0)
	, m_type(0)
{
}
Item::~Item()
{
}



void Item::Initialize(const UINT & type, const D3DXVECTOR2 & position)
{
	m_position = position;
	m_type = type;

	m_aabb.SetPosition(m_position);
	m_aabb.SetSize(D3DXVECTOR2(13, 13));
}

void Item::Update()
{
}

void Item::Render()
{
	std::wstring key;
	switch (m_type)
	{
	case 0:	key = L"Item_Burst";	break;
	case 1:	key = L"Item_PowerUp";	break;
	case 2:	key = L"Item_Grenade";	break;
	}
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(key), D3DXVECTOR3(m_position.x, m_position.y, 0));
}

void Item::Release()
{
}
