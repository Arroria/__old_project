#pragma once
class Ship;
class Lockoner
{
private:
	Ship* m_launcher;
	POINT m_lockonPos;
	bool m_byPlayer;

	bool m_renderOn;

	UINT m_stay;
	std::pair<UINT, UINT> m_frame;
	bool m_isReadyToFire;

public:
	Ship* GetLauncher() { return m_launcher; }
	POINT GetLockonPos() { return m_lockonPos; }
	bool ByPlayer() { return m_byPlayer; }
	bool IsReadyToFire() { return m_isReadyToFire; }

public:
	void Update();
	void Render(const D3DXVECTOR3& pos);

public:
	Lockoner(Ship* launcher, const POINT& lockonPos, const bool& byPlayer, const UINT& stay = 0, const bool& renderOn = true);
	~Lockoner();
};

