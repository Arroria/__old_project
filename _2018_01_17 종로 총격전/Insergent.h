#pragma once

class Attack;
class Insergent
{
private:
	D3DXVECTOR2 m_position;
	UINT m_attackPattern;
	UINT m_attackCount;
	UINT m_wait;
	
	UINT m_animation;

	UINT m_health;
	UINT m_stun;
	Collision_box m_aabb;

	std::vector<Attack*>* m_attackContainer;

public:
	void Hit() { if(m_health) m_health--; if (!m_health) m_stun = 420; }
	UINT GetHealth() { return m_health; }
	Collision_box& GetAABB() { return m_aabb; }
	D3DXVECTOR2 GetPosition() { return m_position; }

public:
	void Initialize(std::vector<Attack*>* attackContainer);
	void Update(const D3DXVECTOR2& cameraPosition, const D3DXVECTOR2& cameraSize);
	void Render();
	void Release();

public:
	Insergent();
	~Insergent();
};

