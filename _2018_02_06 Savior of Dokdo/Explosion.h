#pragma once
#include "Effect3D.h"
class Explosion :
	public Effect3D
{
private:
	D3DXVECTOR3 m_pos;
	UINT m_frame;

public:
	void Update() override;
	void Render() override;

public:
	Explosion(const D3DXVECTOR3& pos);
	~Explosion();
};

