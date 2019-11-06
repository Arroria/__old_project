#include "stdafx.h"
#include "CameraManager.h"



CameraData::CameraData()
	: isPerspective(false)
	
	, position(0, 0, 0)
	, direction(0, 0, 1)
	, up(0, 1, 0)
	
	, viewNearRange(0)
	, viewFarRange(1)
	
	, widthRange(2)
	, heightRange(2)
	
	, sightAngle(45)
	, aspectRatio(1)
{
}

CameraManager::CameraManager()
	: m_settingRunning(false)
{
}
CameraManager::~CameraManager()
{
}



void CameraManager::BeginSetting()
{
	m_settingRunning = true;
}

void CameraManager::EndSetting(LPDIRECT3DDEVICE9 device)
{
	m_settingRunning = false;
	SettingApply(device);
}

void CameraManager::SettingApply(LPDIRECT3DDEVICE9 device)
{
	D3DXMATRIX vM, pM;
	D3DXMatrixLookAtLH(&vM, &m_cameraData.position, &(m_cameraData.position + m_cameraData.direction), &m_cameraData.up);
	if (m_cameraData.isPerspective)
		D3DXMatrixPerspectiveFovLH(&pM, m_cameraData.sightAngle, m_cameraData.aspectRatio, m_cameraData.viewNearRange, m_cameraData.viewFarRange);
	else
		D3DXMatrixOrthoOffCenterLH(&pM,
			m_cameraData.position.x - m_cameraData.widthRange * 0.5f,
			m_cameraData.position.x + m_cameraData.widthRange * 0.5f,
			m_cameraData.position.y - m_cameraData.widthRange * 0.5f,
			m_cameraData.position.y + m_cameraData.widthRange * 0.5f,
			m_cameraData.viewNearRange,
			m_cameraData.viewFarRange);

	device->SetTransform(D3DTS_VIEW, &vM);
	device->SetTransform(D3DTS_PROJECTION, &pM);
}
