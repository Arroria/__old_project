#pragma once
#include "Effect.h"
class ScanEffect :
	public Effect
{
private:
	std::pair<UINT, UINT> m_frame;
	D3DXVECTOR3 m_pos;

public:
	void Update();
	void Render();

public:
	ScanEffect(const D3DXVECTOR3& pos);
	~ScanEffect();
};

