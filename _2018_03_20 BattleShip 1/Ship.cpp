#include "DXUT.h"
#include "Ship.h"


Ship::Ship(const UINT& size)
	: m_size(size)
	, m_life(size)

	, m_ase(ASEGet( L"x" + std::to_wstring(size) + L"Ship"))
{
}
Ship::~Ship()
{
}



void Ship::Assign(const POINT & tile, const bool & lookAtRight)
{
	m_modelPos = tile;
	m_lookAtRight = lookAtRight;
}

bool Ship::Hit()
{
	if (m_life)
	{
		m_life--;
		return m_life;
	}
	assert("Ship is -HP");
	return false;
}

void Ship::Attack(const POINT & target)
{
	//¹Ì¿Ï¼º
}

void Ship::Explosion()
{
}


void Ship::Render(const D3DXMATRIX& worldM)
{
	D3DXMATRIX sm, rm, tm;
	D3DXMatrixScaling(&sm, 1, 1, 1);
	D3DXMatrixRotationY(&rm, D3DXToRadian(m_lookAtRight ? -90 : 0));
	D3DXMatrixTranslation(&tm, m_modelPos.x - 4.5, 0, 4.5 - m_modelPos.y);
	DEVICE->SetTransform(D3DTS_WORLD, &(sm * rm * tm * worldM));
	m_ase->Render();
}
