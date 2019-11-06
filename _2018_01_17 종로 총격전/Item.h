#pragma once
class Item
{
private:
	D3DXVECTOR2 m_position;
	UINT m_type;

	Collision_box m_aabb;

public:
	Collision_box& GetAABB() { return m_aabb; }
	UINT GetType() { return m_type; }

public:
	void Initialize(const UINT& type, const D3DXVECTOR2& position);
	void Update();
	void Render();
	void Release();

public:
	Item();
	~Item();
};

