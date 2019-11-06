#pragma once
#include "Effect.h"
class AttackHit :
	public Effect
{
private:
	D3DXVECTOR3 m_pos;

	std::pair<UINT, UINT> m_frame;

public:
	void Update() override;
	void Render() override;

public:
	AttackHit(const D3DXVECTOR3& pos);
	~AttackHit();
};

