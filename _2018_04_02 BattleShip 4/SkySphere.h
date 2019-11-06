#pragma once
#include "Singleton.h"
class SkySphere :
	public Singleton<SkySphere>
{
private:
	UINT m_frame;

public:
	void Update();
	void Render();

public:
	SkySphere();
	~SkySphere();
};

