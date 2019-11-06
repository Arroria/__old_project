#pragma once
class House
{
private:
	D3DXVECTOR2 m_position;
	Collision_Line m_collisionList;
	
	Texture* m_tex;

public:
	Collision_Line& GetLine() { return m_collisionList; }

	D3DXVECTOR2 GetPosition() { return m_position; }
	void SetPosition(const D3DXVECTOR2& position) { m_position = position; }

public:
	void Initialize(const D3DXVECTOR2& position, const UINT& type);
	void Update();
	void Render();
	void Release();

public:
	House();
	~House();
};

