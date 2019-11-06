#pragma once
#include "Singleton.h"
class Camera :
	public Singleton<Camera>
{
private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_dir;
	float m_viewRange;

	D3DXVECTOR2 m_cameraSize;

public:
	void SetPos	(const D3DXVECTOR3& pos)	{ m_pos = pos; }
	void SetDir	(const D3DXVECTOR3& dir)	{ m_dir = dir; }
	void SetAt	(const D3DXVECTOR3& at)		{ D3DXVec3Normalize(&m_dir, &(at - m_pos)); }
	void SetViewRange(const float& viewRange) { m_viewRange = viewRange; }

	void SetCameraSize(const D3DXVECTOR2& cameraSize) { m_cameraSize = cameraSize; }

	void SetMatrix();

public:
	Camera();
	~Camera();
};

#define CAMERA SingletonInstance(Camera)
