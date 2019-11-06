#pragma once
class Scan
{
private:
	bool m_isExpired;
public:
	bool IsExpired() { return m_isExpired; }
	void Expire() { m_isExpired = true; }

private:
	D3DXVECTOR3 m_pos;
	POINT m_lockonPos;
	UINT m_frame;
	bool m_isPlayerAttack;

public:
	POINT GetLockPos() { return m_lockonPos; }
	bool IsPlayerAttack() { return m_isPlayerAttack; }
	UINT GetFrame() { return m_frame; }

public:
	void Update();
	void Render();

public:
	Scan(const D3DXVECTOR3& pos, const POINT& lockonPos, const bool& isPlayerAttack);
	~Scan();
};

