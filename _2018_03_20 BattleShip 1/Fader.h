#pragma once
#include "Singleton.h"
class Fader :
	public Singleton<Fader>
{
private:
	bool m_in;
	UINT m_frame;
	UINT m_stay;

public:
	bool IsIn() { return m_in; }
	bool IsRunning() { return m_frame; }

	void SetStay(const UINT& time) { m_stay = time; }

public:
	void Initialize();
	void Create(const bool& in);
	bool Update();
	void Render();

public:
	Fader();
	~Fader();
};

