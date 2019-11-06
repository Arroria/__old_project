#pragma once
#include "Singleton.h"
class Camera :
	public Singleton<Camera>
{
private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_dir;

	float m_ratio;
	float m_aspect;
	float m_near;
	float m_far;

public:
	void SetView(float ratio, float aspect, float _near, float _far);
	
	void SetPos	(const D3DXVECTOR3& pos);
	void SetAt	(const D3DXVECTOR3& at);
	void SetDir	(const D3DXVECTOR3& dir);

	void Assign();

public:
	Camera();
	~Camera();
};

