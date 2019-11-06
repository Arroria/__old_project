#include "DXUT.h"
#include "Camera.h"


Camera::Camera()
	: m_pos(0, 0, 0)
	, m_dir(0, 0, 1)
	, m_ratio(1)
	, m_aspect(D3DX_PI / 2)
	, m_near(0)
	, m_far(1)
{
}
Camera::~Camera()
{
}



void Camera::SetView(float ratio, float aspect, float _near, float _far)
{
	m_ratio = ratio;
	m_aspect = aspect;
	m_near = _near;
	m_far = _far;
}

void Camera::SetPos(const D3DXVECTOR3 & pos)
{
	m_pos = pos;
}

void Camera::SetAt(const D3DXVECTOR3 & at)
{
	D3DXVECTOR3 temp;
	m_dir = *D3DXVec3Normalize(&temp, &(at - m_pos));
}

void Camera::SetDir(const D3DXVECTOR3 & dir)
{
	m_dir = dir;
}

void Camera::Assign()
{
	D3DXMATRIX v, p;
	D3DXMatrixLookAtLH(&v, &m_pos, &(m_pos + m_dir), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixPerspectiveFovLH(&p, m_ratio, m_aspect, m_near, m_far);
	DEVICE->SetTransform(D3DTS_VIEW, &v);
	DEVICE->SetTransform(D3DTS_PROJECTION, &p);
}
