#pragma once
class Missile
{
private:
	bool m_isExpired;
public:
	bool IsExpired() { return m_isExpired; }
	void Expire() { m_isExpired = true; }

private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_dir;
	
	D3DXVECTOR3 m_objPos;
	bool m_isFalling;

	POINT m_lockonPos;
	bool m_byPlayer;

public:
	D3DXVECTOR3 GetPos() { return m_pos; }
	POINT GetLockonPos() { return m_lockonPos; }
	bool IsByPlayer() { return m_byPlayer; }
	bool IsFalling() { return m_isFalling; }

	void SetPath(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir, const D3DXVECTOR3& objPos);
	void Falling() { m_isFalling = true; }

public:
	void Update();
	void Render();

public:
	Missile(const POINT& lockonPos, const bool& byPlayer);
	~Missile();
};

