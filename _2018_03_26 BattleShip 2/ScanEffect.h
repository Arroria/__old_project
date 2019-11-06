#pragma once
#include "Effect.h"
class ScanEffect :
	public Effect
{
private:
	D3DXVECTOR3 m_pos;
	std::pair<UINT, UINT> m_frame;

public:
	void Update() override;
	void Render() override;

public:
	ScanEffect(const D3DXVECTOR3& pos);
	~ScanEffect();
};

