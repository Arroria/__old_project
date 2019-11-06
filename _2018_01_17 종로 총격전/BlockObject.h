#pragma once
class BlockObject
{
private:
	D3DXVECTOR2 m_position;
	Collision_box m_aabb;
	Texture* m_texture;

public:
	Collision_box& GetAABB() { return m_aabb; }
	D3DXVECTOR2 GetPosition() { return m_position; }

public:
	void Initialize(const D3DXVECTOR2& position, const D3DXVECTOR2& collisionSize, Texture* texture);
	void Update();
	void Render();
	void Release();

public:
	BlockObject();
	~BlockObject();
};

