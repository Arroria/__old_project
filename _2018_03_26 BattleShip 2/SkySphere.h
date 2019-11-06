#pragma once
#include "Singleton.h"
class SkySphere :
	public Singleton<SkySphere>
{
private:
	float m_rotate;

public:
	void Update();
	void Render();

public:
	SkySphere();
	~SkySphere();
};

