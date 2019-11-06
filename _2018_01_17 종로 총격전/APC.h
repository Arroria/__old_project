#pragma once

class Player;
class Attack;
class Effect;
class APC
{
private:
	D3DXVECTOR2 m_position;
	bool m_lookAtRight;
	UINT m_state;

	UINT m_animation;

	UINT m_health;
	Collision_box m_aabb;

	float m_prevAngle;
	D3DXQUATERNION m_barrelRotation;

	const Player* m_player;
	std::vector<Attack*>* m_attackContainer;
	std::list<Effect*>* m_effectContainer;

public:
	void Hit() { if (m_health) m_health--; m_state = 2; }

public:
	Collision_box& GetAABB() { return m_aabb; }
	UINT GetHealth() { return m_health; }
	D3DXVECTOR2 GetPosition() { return m_position; }
	UINT GetState() { return m_state; }

public:
	void Initialize(std::vector<Attack*>* attackContainer, std::list<Effect*>* effectContainer, const Player* player);
	void Update();
	void Render();
	void Release();

public:
	APC();
	~APC();
};

