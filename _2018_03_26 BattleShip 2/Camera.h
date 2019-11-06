#pragma once
#include "Singleton.h"
class Camera :
	public Singleton<Camera>
{
private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_dir;

	float m_aspect;
	float m_viewAngle;
	float m_nearRange;
	float m_farRange;

public:
	void Initialize(const float& aspect, const float& viewAngle, const float& nearRange, const float& farRange);
	void Transform(const D3DXVECTOR3* pos, const D3DXVECTOR3* at = nullptr, const D3DXVECTOR3* dir = nullptr);
	void ApplyDevice(LPDIRECT3DDEVICE9 device);

public:
	Camera();
	~Camera();
};

