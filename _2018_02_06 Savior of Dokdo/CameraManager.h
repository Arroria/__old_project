#pragma once
#include "Singleton.h"

struct CameraData
{
	bool isPerspective;

	//CameraData
	D3DXVECTOR3 position;
	D3DXVECTOR3 direction;
	D3DXVECTOR3 up;

	//ViewData
	float viewNearRange;
	float viewFarRange;

	//Ortho
	float widthRange;
	float heightRange;

	//Perspective
	float sightAngle;
	float aspectRatio;

	CameraData();
};

class CameraManager :
	public Singleton<CameraManager>
{
private:
	bool m_settingRunning;
	CameraData m_cameraData;
public:
	void SetProjection	(const bool& perspective)			{ if (m_settingRunning)	m_cameraData.isPerspective = perspective; }
	void SetCameraPosition	(const D3DXVECTOR3& position)	{ if (m_settingRunning)	m_cameraData.position = position; }
	void SetCameraDirection	(const D3DXVECTOR3& direction)	{ if (m_settingRunning)	m_cameraData.direction = direction; }
	void SetCameraUp		(const D3DXVECTOR3& up)			{ if (m_settingRunning)	m_cameraData.up = up; }
	void SetCameraAt		(const D3DXVECTOR3& at)			{ if (m_settingRunning)	D3DXVec3Normalize(&m_cameraData.direction, &(at - m_cameraData.position)); }
	void SetViewRangeNear	(const float& range)			{ if (m_settingRunning)	m_cameraData.viewNearRange = range; }
	void SetViewRangeFar	(const float& range)			{ if (m_settingRunning)	m_cameraData.viewFarRange = range; }
	void SetViewRangeWidth	(const float& range)			{ if (m_settingRunning)	m_cameraData.widthRange = range; }
	void SetViewRangeHeight	(const float& range)			{ if (m_settingRunning)	m_cameraData.heightRange = range; }
	void SetSightAngle	(const float& sightAngle)			{ if (m_settingRunning)	m_cameraData.sightAngle = sightAngle; }
	void SetAspectRatio	(const float& aspectRatio)			{ if (m_settingRunning)	m_cameraData.aspectRatio = aspectRatio; }

			CameraData	GetData()		{ return m_cameraData; }
	const	CameraData&	GetDataRef()	{ return m_cameraData; }

public:
	void BeginSetting();
	void EndSetting(LPDIRECT3DDEVICE9 device);
	void SettingApply(LPDIRECT3DDEVICE9 device);

public:
	CameraManager();
	~CameraManager();
};
