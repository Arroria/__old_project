#pragma once
#include "Effect.h"
class Satellite :
	public Effect
{
private:
	float m_length;
	float m_rot1;
	float m_rot2;

public:
	void Update();
	void Render();

public:
	Satellite();
	~Satellite();
};

