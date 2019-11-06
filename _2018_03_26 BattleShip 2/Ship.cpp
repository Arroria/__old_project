#include "DXUT.h"
#include "Ship.h"


Ship::Ship(const UINT & size)
	: m_size(size)
	, m_life(size)

	, m_tilePos{ -1, -1 }
	, m_lookAtY(false)
{
}
Ship::~Ship()
{
}



void Ship::Placed(const POINT & tilePos, const bool & lookAtY)
{
	m_tilePos = tilePos;
	m_lookAtY = lookAtY;
}

void Ship::Render(const D3DXMATRIX& worldM)
{
	D3DXMATRIX tm;
	D3DXMatrixIdentity(&tm);

	D3DXMATRIX myM;
	myM = tm * worldM;

	DEVICE->SetTransform(D3DTS_WORLD, &myM);
	switch (m_size)
	{
	case 2:	ASEGet(L"T2")->Render();	break;
	case 3:	ASEGet(L"T3")->Render();	break;
	case 4:	ASEGet(L"T4")->Render();	break;
	}
}

