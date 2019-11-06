#pragma once

class Ship;
class Lockon
{
private:
	std::pair<UINT, UINT> m_frame;
	UINT m_stay;

	Ship* m_launcher;
	POINT m_lockonPos;
	bool m_byPlayer;
	bool m_lockonComplate;

public:
	POINT GetLockonPos()	{ return m_lockonPos; }
	Ship* GetLauncher()		{ return m_launcher; }
	bool ByPlayer()			{ return m_byPlayer; }
	bool LockonComplate()	{ return m_lockonComplate; }

public:
	void Update();
	void Render(const D3DXVECTOR3& pos);

public:
	Lockon(Ship* launcher, const POINT& lockonPos, const bool& byPlayer, const UINT& stay = 0);
	~Lockon();
};

