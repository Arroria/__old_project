#pragma once
class Grenade
{
private:
	D3DXVECTOR2 m_position;
	D3DXQUATERNION m_rotation;
	float m_speed;

	bool m_moveToLeft;
	float m_gravity;

	Collision_box m_aabb;

	bool m_isExpired;

public:
	void Expire() { m_isExpired = true; }
	bool IsExpired() { return m_isExpired; }

	Collision_box& GetAABB() { return m_aabb; }

	D3DXVECTOR2	GetPosition() { return m_position; }

public:
	void Initialize(const D3DXVECTOR2& position, const bool& moveToLeft);
	void Update();
	void Render();
	void Release();

public:
	Grenade();
	~Grenade();
};

