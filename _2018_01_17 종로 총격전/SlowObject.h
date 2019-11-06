#pragma once
class SlowObject
{
private:
	D3DXVECTOR2 m_position;
	Collision_box m_aabb;

	UINT m_health;

	Texture* m_tex;

public:
	void Hit() { if (m_health) m_health--; }
	UINT GetHealth() { return m_health; }
	D3DXVECTOR2 GetPosition() { return m_position; }
	Collision_box& GetAABB() { return m_aabb; }

public:
	void Initialize(const D3DXVECTOR2& position);
	void Update();
	void Render();
	void Release();

public:
	SlowObject();
	~SlowObject();
};

