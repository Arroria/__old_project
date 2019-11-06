#pragma once

class Effect;
class Missile
{
private:
	bool m_isExpired;
public:
	void Expire() { m_isExpired = true; }
	bool IsExpired() { return m_isExpired; }


private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_dir;
	D3DXVECTOR3 m_objectivePos;
	D3DXVECTOR3 m_terminal;

	bool m_byPlayer;
	bool m_diving;
	bool m_hitEffect;

	float m_speed;

public:
	D3DXVECTOR3 GetPos() { return m_pos; }
	void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	void SetDir(const D3DXVECTOR3& dir) { m_dir = dir; }
	bool IsByPlayer() { return m_byPlayer; }
	bool IsDiving() { return m_diving; }
	bool isHit() { return m_hitEffect; }
	void Diving() { m_diving = true; m_objectivePos = m_terminal; }

public:
	void Update(std::list<Effect*>& effectList);
	void Render();

public:
	Missile(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir, const D3DXVECTOR3& objectivePos, const D3DXVECTOR3& terminal, const bool& byPlayer, const bool& isHit);
	~Missile();
};

