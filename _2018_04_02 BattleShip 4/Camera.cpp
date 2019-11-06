#include "DXUT.h"
#include "Camera.h"


Camera::Camera()
	: m_pos(0, 0, 0)
	, m_dir(0, 0, 1)
{
}
Camera::~Camera()
{
}



void Camera::AssignData()
{
	D3DXMATRIX view, proj;
	D3DXMatrixLookAtLH(&view, &m_pos, &(m_pos + m_dir), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(45), (float)WND_SIZEX / WND_SIZEY, 0.1, 250);
	DEVICE->SetTransform(D3DTS_VIEW, &view);
	DEVICE->SetTransform(D3DTS_PROJECTION, &proj);
}



void Camera::SetPos(const D3DXVECTOR3 & pos)
{
	m_pos = pos;
}

void Camera::SetAt(const D3DXVECTOR3 & at)
{
	D3DXVec3Normalize(&m_dir, &(at - m_pos));
}

void Camera::SetDir(const D3DXVECTOR3 & dir)
{
	m_dir = dir;
}
