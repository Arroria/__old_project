#pragma once
#include "Singleton.h"
class Camera :
	public Singleton<Camera>
{
private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_dir;

public:
	void SetPos	(const D3DXVECTOR3& pos);
	void SetAt	(const D3DXVECTOR3& at);
	void SetDir	(const D3DXVECTOR3& dir);

public:
	void AssignData();

public:
	Camera();
	~Camera();
};

