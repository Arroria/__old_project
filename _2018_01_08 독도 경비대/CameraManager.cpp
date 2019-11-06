#include "stdafx.h"
#include "CameraManager.h"


Camera::Camera()
	: position(0, 0, 0)
	, at(0, 0, 1)
	, up(0, 1, 0)
	
	, fov(D3DXToRadian(45))
	, aspect(1)
	, nearRange(.1)
	, farRange(1000)
	
	, changed(false)
{
	D3DXMatrixIdentity(&view);
	D3DXMatrixIdentity(&projection);
}
Camera::~Camera()
{
}


void Camera::SetCameraView(const D3DXVECTOR3 * _position, const D3DXVECTOR3 * _at, const D3DXVECTOR3 * _up)
{
	if (_position)	{ changed = true; position = *_position; }
	if (_at)		{ changed = true; at = *_at; }
	if (_up)		{ changed = true; up = *_up; }
}

void Camera::SetCameraProjection(const float& _fov, const float& _aspect, const float& _nearRange, const float& _farRange)
{
	fov = _fov;
	aspect = _aspect;
	nearRange = _nearRange;
	farRange = _farRange;
	
	changed = true; 
}

HRESULT Camera::MatrixInput(LPDIRECT3DDEVICE9 device)
{
	if (changed)
	{
		D3DXMatrixLookAtLH(&view, &position, &at, &up);
		D3DXMatrixPerspectiveFovLH(&projection, fov, aspect, nearRange, farRange);
	}

	if (SUCCEEDED( device->SetTransform(D3DTS_VIEW, &view) ) &&
		SUCCEEDED( device->SetTransform(D3DTS_PROJECTION, &projection) ))
		return S_OK;
	return E_FAIL;
}


CameraManager::CameraManager()
{
}
CameraManager::~CameraManager()
{
}
