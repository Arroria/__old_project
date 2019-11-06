#include "DXUT.h"
#include "Camera.h"


Camera::Camera()
	: m_pos(0, 0, 0)
	, m_dir(0, 0, 1)

	, m_aspect(0)
	, m_viewAngle(0)
	, m_nearRange(0)
	, m_farRange(0)
{
}
Camera::~Camera()
{
}



void Camera::Initialize(const float & aspect, const float & viewAngle, const float & nearRange, const float & farRange)
{
	m_aspect	= aspect	;
	m_viewAngle	= viewAngle	;
	m_nearRange	= nearRange	;
	m_farRange	= farRange	;
}

void Camera::Transform(const D3DXVECTOR3 * pos, const D3DXVECTOR3 * at, const D3DXVECTOR3 * dir)
{
	if (pos)
		m_pos = *pos;

	if (at)
		D3DXVec3Normalize(&m_dir, &(*at - m_pos));
	else if (dir)
		m_dir = *dir;
}

void Camera::ApplyDevice(LPDIRECT3DDEVICE9 device)
{
	D3DXMATRIX view, proj;
	D3DXMatrixLookAtLH(&view, &m_pos, &m_dir, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixPerspectiveFovLH(&proj, m_viewAngle, m_aspect, m_nearRange, m_farRange);
	DEVICE->SetTransform(D3DTS_VIEW, &view);
	DEVICE->SetTransform(D3DTS_PROJECTION, &proj);
}
