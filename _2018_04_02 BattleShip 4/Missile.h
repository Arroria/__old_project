#pragma once
class Missile
{
private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_dir;
	D3DXVECTOR3 m_terminal;
	bool m_isDiving;

	POINT m_lockonPos;
	bool m_byPlayer;

public:
	D3DXVECTOR3 GetPos() { return m_pos; }
	float GetHeight() { return m_pos.y; }
	bool IsDiving() { return m_isDiving; }
	
	POINT GetLockonPos() { return m_lockonPos; }
	bool ByPlayer() { return m_byPlayer; }

	void SetState(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir, const D3DXVECTOR3& terminal, const bool& isDiving);

public:
	void Update();
	void Render();

public:
	Missile(const POINT& lockonPos, const bool& byPlayer);
	~Missile();
};

