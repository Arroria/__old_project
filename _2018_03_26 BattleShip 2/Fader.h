#pragma once
#include "Singleton.h"
class Fader :
	public Singleton<Fader>
{
private:
	UINT m_frame;
	UINT m_stay;
	bool m_isIn;

public:
	void Create(const bool& isIn, const UINT& stay = 0);

	bool IsIn() { return m_isIn; }
	bool IsRun() { return m_frame || m_stay; }

public:
	bool Update();
	void Render();

public:
	Fader();
	~Fader();
};

