#pragma once
class Lockon
{
private:
	bool m_isExpired;
public:
	bool IsExpired() { return m_isExpired; }
	void Expire() { m_isExpired = true; }

private:
	POINT m_lockonPos;
	std::pair<UINT, UINT> m_frame;
	UINT m_stay;
	bool m_isPlayerAttack;

	Channel* m_sound;

public:
	POINT GetLockPos() { return m_lockonPos; }
	bool IsStay() { return m_stay; }
	bool IsPlayerAttack() { return m_isPlayerAttack; }

public:
	void Update();
	Texture* GetTexture();

public:
	Lockon(const POINT& lockonPos, const bool& isPlayerAttack, const UINT& stay = 0);
	~Lockon();
};

