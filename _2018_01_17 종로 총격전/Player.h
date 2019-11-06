#pragma once
class Attack;
class Grenade;
class Player
{
private:
	D3DXVECTOR2 m_position;

	bool m_lookAtLeft;
	float m_gravity;
	bool m_isGround;
	bool m_isDoubleJumped;
	bool m_autoMovingActivate;
	UINT m_slow;

	D3DXQUATERNION m_gunRotation;
	std::pair<UINT, UINT> m_gunRebound;
	UINT m_specialAttack;
	UINT m_specialAttackTime;
	UINT m_grenadeCount;

	UINT m_walking;

	UINT m_health;
	Collision_box m_aabb;

	const Camera& m_camera;
	std::vector<Attack*>* m_attackContainer;
	std::vector<Grenade*>* m_grenadeContainer;

private:
	void Move();
	void Aiming();
	void _Attack();

public:
	Collision_box& GetAABB() { return m_aabb; }

	D3DXVECTOR2 GetPosition() const { return m_position; }
	void SetPosition(const D3DXVECTOR2& position) { m_position = position; }
	void AutoMovingActivate(const bool& activate = true) { m_autoMovingActivate = activate; }

	float GetGravity() const { return m_gravity; }
	void SetGravity(const float& gravity) { m_gravity = gravity; }
	bool IsGround() { return m_isGround; }
	void SetGround() { m_isGround = true; m_isDoubleJumped = false; }
	void SetSlow() { m_slow = 180; }

	void Hit() { if (m_health) m_health--; }
	void Kill() { m_health = 0; }
	UINT GetHealth() { return m_health; }

	void SetSpecialAttack(const UINT& type) { m_specialAttack = type; m_specialAttackTime = 600; }
	UINT GetSpecialAttack() { return m_specialAttack; }
	UINT GetSpecialAttackTime() { return m_specialAttackTime; }
	UINT GetGrenadeCount() { return m_grenadeCount; }
	void SetGrenadeCount(const UINT& count) { m_grenadeCount = count; }
	void InsertGrenade(const UINT& count = 1) { m_grenadeCount += count; if (m_grenadeCount > 9) m_grenadeCount = 9; }

public:
	void Initialize();
	void Update();
	void Render();
	void Release();

public:
	Player(const Camera& camera, std::vector<Attack*>* attackContainer, std::vector<Grenade*>* grenadeContainer);
	~Player();
};

