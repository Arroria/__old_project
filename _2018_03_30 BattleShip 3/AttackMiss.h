#pragma once
#include "Effect.h"
class AttackMiss :
	public Effect
{
private:
	D3DXVECTOR3 m_pos;

	std::pair<UINT, UINT> m_frame;

public:
	void Update() override;
	void Render() override;

public:
	AttackMiss(const D3DXVECTOR3& pos);
	~AttackMiss();
};

