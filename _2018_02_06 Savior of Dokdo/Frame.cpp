#include "stdafx.h"
#include "Frame.h"


Frame::Frame()
	: m_position(0, 0, 0)
	, m_rollRotation(0, 0, 0, 1)
	, m_yawRotation(0, 0, 0, 1)
	
	, m_speed(0)
{
}
Frame::~Frame()
{
}



D3DXVECTOR3 Frame::GetDirection() const
{
	D3DXVECTOR3 direction;
	D3DXMATRIX r;
	D3DXMatrixRotationQuaternion(&r, &m_yawRotation);
	D3DXVec3TransformCoord(&direction, &D3DXVECTOR3(0, 0, 1), &r);
	return direction;
}