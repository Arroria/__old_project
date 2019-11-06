#include "DXUT.h"
#include "Ship.h"


Ship::Ship(const UINT & size)
	: m_size(size)
	, m_life(size)

	, m_tilePos{ -1, -1 }
	, m_pos(0, 0, 0)

	, m_frame(0)
{
}
Ship::~Ship()
{
}



void Ship::Update(const D3DXVECTOR3 & pos)
{
	m_frame++;

	m_pos = pos;
	m_pos.y += 1 + sinf(D3DXToRadian( m_frame * 3 )) * 0.4f;
}

void Ship::Render()
{
	D3DXMATRIX rm, tm;
	D3DXMatrixRotationY(&rm, D3DXToRadian(m_isLookAtY ? 0 : 90));
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);

	DEVICE->SetTransform(D3DTS_WORLD, &(rm * tm));
	switch (m_size)
	{
	case 2:	ASEGet(L"ShipX2")->Render();	break;
	case 3:	ASEGet(L"ShipX3")->Render();	break;
	case 4:	ASEGet(L"ShipX4")->Render();	break;
	}
}



void Ship::SetPlaced(const POINT & tilePos, const bool & isLookAtY)
{
	m_tilePos = tilePos;
	m_isLookAtY = isLookAtY;
}
