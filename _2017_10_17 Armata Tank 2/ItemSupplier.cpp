#include "stdafx.h"
#include "ItemSupplier.h"
#include "Item.h"

#define DropSpeed 15


ItemSupplier::ItemSupplier(const D3DXVECTOR3 & position, const int & itemId, const float & landingHeight, std::list<Item*>* itemContainer)
	: m_destroyThis(false)

	, m_position(position)
	, m_rotation(0)
	, m_scale(1, 1, 1)

	, m_itemId(itemId)
	, m_landingHeight(landingHeight)
	
	, m_itemContainer(itemContainer)
{
}
ItemSupplier::~ItemSupplier()
{
}


void ItemSupplier::Update()
{
	if (m_position.z <= m_landingHeight)
	{
		m_position.z = m_landingHeight;
		m_itemContainer->push_back(new Item(m_itemId, m_position));
		DestroyThis();
	}
	else
		m_position.z -= g_processManager->GetDeltaTime() * DropSpeed;
}

void ItemSupplier::Render()
{
	D3DXMATRIX s, r, t;
	D3DXMatrixScaling(&s, m_scale.x, m_scale.z, m_scale.y);
	D3DXMatrixRotationY(&r, -D3DXToRadian(m_rotation));
	D3DXMatrixTranslation(&t, m_position.x, m_position.z, m_position.y);

	g_ase->GetAse(L"ItemSupplier")->Render(s * r * t);
}
