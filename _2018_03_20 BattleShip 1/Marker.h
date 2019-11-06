#pragma once
#include "Effect.h"
class Marker :
	public Effect
{
private:
	bool m_hit;

	D3DXVECTOR3 m_pos;
	float m_angle;

public:
	void Update();
	void Render();

public:
	Marker(const D3DXVECTOR3& pos, const bool& isHit);
	~Marker();
};

