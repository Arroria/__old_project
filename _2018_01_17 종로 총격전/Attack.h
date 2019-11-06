#pragma once
class Attack
{
private:
	Texture* m_texture;

	D3DXVECTOR2 m_position;
	D3DXQUATERNION m_rotation;
	float m_speed;

	bool m_isFake;
	Collision_box m_aabb;

	UINT m_duration;
	bool m_isExpired;

public:
	void Expire() { m_isExpired = true; }
	bool IsExpired() { return m_isExpired; }

	bool IsFake() { return m_isFake; }
	Collision_box& GetAABB() { return m_aabb; }

	D3DXVECTOR2	GetPosition() { return m_position; }

public:
	void Initialize(Texture* texture, const D3DXVECTOR2& position, const D3DXQUATERNION& rotation, const float& speed, const bool& isFake = false);
	void Update();
	void Render();
	void Release();

public:
	Attack();
	~Attack();
};

