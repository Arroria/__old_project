#pragma once
#include "Effect.h"
class ExplosionHit :
	public Effect
{
private:
	D3DXVECTOR3 m_pos;

	UINT m_frame;
	UINT m_slowFrame;

public:
	void Update();
	void Render();

public:
	ExplosionHit(const D3DXVECTOR3& pos);
	~ExplosionHit();
};

