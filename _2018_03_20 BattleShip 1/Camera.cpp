#include "DXUT.h"
#include "Camera.h"


Camera::Camera()
	: m_pos(0, 0, 0)
	, m_dir(0, 0, 1)
	, m_viewRange(100)

	, m_cameraSize(1600, 900)
{
}
Camera::~Camera()
{
}



void Camera::SetMatrix()
{
	D3DXMATRIX viewM, projM;
	D3DXMatrixLookAtLH(&viewM, &m_pos, &(m_pos + m_dir), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixPerspectiveFovLH(&projM, D3DXToRadian(45), 16. / 9, 0.001, m_viewRange);
	DEVICE->SetTransform(D3DTS_VIEW, &viewM);
	DEVICE->SetTransform(D3DTS_PROJECTION, &projM);
}
