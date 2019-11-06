#pragma once
class Attack;
class Player;
class Enemy
{
private:
	D3DXVECTOR2 m_position;

	bool m_lookAtRight;
	float m_gravity;
	int m_randomMovingCooltime;
	int m_randomMovingDuration;
	bool m_randomMovingToRight;
	CONST UINT m_courage;

	UINT m_walking;

	D3DXQUATERNION m_gunRotation;
	UINT m_gunRebound;

	UINT m_health;
	Collision_box m_aabb;

	UINT m_moveType;

	const Player* m_player;
	std::vector<Attack*>* m_attackContainer;

public:
	D3DXVECTOR2 GetPosition() { return m_position; }
	void SetPosition(const D3DXVECTOR2& position) { m_position = position; }
	bool IsLookAtRight() { return m_lookAtRight; }

	float GetGravity() { return m_gravity; }
	void SetGravity(const float& gravity) { m_gravity = gravity; }

	void Hit(const UINT& damage = 1) { if (m_health) { if (m_health >= damage) m_health -= damage; else m_health = 0; } }
	void Kill() { m_health = 0; }
	void SetHealth(const UINT& health) { m_health = health; }
	UINT GetHealth() { return m_health; }
	Collision_box& GetAABB() { return m_aabb; }

private:
	void Move1();
	void Move2();
	void Move3();
	void Aiming();
	void _Attack();

public:
	void Initialize(const Player* player, std::vector<Attack*>* attackContainer, const UINT& type = 0);
	void Update();
	void Render();
	void Release();

public:
	Enemy();
	~Enemy();
};

