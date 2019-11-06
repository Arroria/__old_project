#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
	: m_position(0, 0, 0)
	, m_size(1, 1, 1)
{
}
Camera::~Camera()
{
}



void Camera::SetCameraTranslation(const D3DXVECTOR3 * position, const D3DXVECTOR3 * size)
{
	if (position)	m_position	= *position;
	if (size)		m_size		= *size;
}

void Camera::InputCameraDevice(LPDIRECT3DDEVICE9 device)
{
	D3DXMATRIX view, projection;
	D3DXMatrixLookAtLH(&view,
		&D3DXVECTOR3(m_position.x, m_position.y, 0),
		&D3DXVECTOR3(m_position.x, m_position.y, 1),
		&D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoOffCenterLH(&projection,
		-m_size.x, m_size.x,
		-m_size.y, m_size.y,
		0, m_size.z);
	device->SetTransform(D3DTS_VIEW, &view);
	device->SetTransform(D3DTS_PROJECTION, &projection);
}
