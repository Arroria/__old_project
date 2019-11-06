#pragma once
class Missile
{
private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_dir;
	D3DXVECTOR3 m_terminal;

	POINT m_lockonPos;
	bool m_byPlayer;
	bool m_isDiving;

public:
	D3DXVECTOR3 GetPos()	{ return m_pos; }
	float GetHeight()		{ return m_pos.y; }
	POINT GetLockonPos()	{ return m_lockonPos; }
	bool ByPlayer()			{ return m_byPlayer; }

	void Resetting(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir, const D3DXVECTOR3& terminal, const bool& isDiving);
	bool IsDiving() { return m_isDiving; }

public:
	void Update();
	void Render();

public:
	Missile(const POINT& lockonPos, const bool& byPlayer);
	~Missile();
};

