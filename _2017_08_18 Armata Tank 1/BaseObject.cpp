#include "MainHeader.h"
#include "BaseObject.h"

namespace AF
{
	BaseObject::BaseObject()
		: m_position(0, 0, 0)
		, m_rotation(0, 0, 0)
		, m_scale(1, 1, 1)
		, m_direction(0, 0, 0)
	{
	}
	BaseObject::~BaseObject()
	{
	}


	D3DXVECTOR3 BaseObject::GetDirection()
	{
		D3DXVECTOR3 direction;
		D3DXMATRIX rotRX, rotRY;
		D3DXMatrixRotationX(&rotRX, D3DXToRadian(m_rotation.x));
		D3DXMatrixRotationY(&rotRY, D3DXToRadian(m_rotation.y));
		D3DXVec3TransformCoord(&direction, &D3DXVECTOR3(0, 0, 1), &(rotRX * rotRY));
		D3DXVec3Normalize(&direction, &direction);
		return direction;
	}

	D3DXMATRIX BaseObject::GetTransformMatrix()
	{
		D3DXMATRIX matS, matRx, matRy, matT;
		D3DXMatrixScaling		(&matS, m_scale.x, m_scale.y, m_scale.z);
		D3DXMatrixRotationX		(&matRx, D3DXToRadian(m_rotation.x));
		D3DXMatrixRotationY		(&matRy, D3DXToRadian(m_rotation.y));
		D3DXMatrixTranslation	(&matT, m_position.x, m_position.y, m_position.z);
		return matS * matRx * matRy * matT;
	}
}