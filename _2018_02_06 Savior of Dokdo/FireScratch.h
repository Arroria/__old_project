#pragma once
#include "SingleTexParticle.h"
class FireScratch :
	public SingleTexParticle
{
private:
	D3DXVECTOR3 m_dir;
	float m_speed;
	UINT m_duraction;
	UINT m_flowTime;

public:
	void Update() override;

public:
	FireScratch(const D3DXVECTOR3& pos, const D3DXCOLOR& color);
	~FireScratch();
};

