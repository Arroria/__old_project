#include "stdafx.h"
#include "BaseShell.h"



BaseShell::BaseShell(const D3DXVECTOR3& position, const D3DXVECTOR3& direction)
	: BaseShell(position, direction, 1, 0)
{
}
BaseShell::BaseShell(const D3DXVECTOR3& position, const D3DXVECTOR3& direction, const float& firePower, const float& penetration)
	: m_destroyThis(false)

	, m_position(position)
	, m_rotation(0)
	, m_scale(1, 1, 1)
	, m_direction(direction)

	, m_firePower(firePower)
	, m_penetration(penetration)
	, m_speed(1)

	, m_collisionSize(NULL, NULL)
{
}
BaseShell::~BaseShell()
{
}


bool BaseShell::Update()
{
	Move();
	return false;
}
