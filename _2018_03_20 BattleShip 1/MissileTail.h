#pragma once
#include "Effect.h"
class MissileTail :
	public Effect
{
private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_dir;

	UINT m_frame;
	UINT m_slowFrame;

public:
	void Update();
	void Render();

public:
	MissileTail(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir);
	~MissileTail();
};

