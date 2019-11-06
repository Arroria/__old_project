#pragma once

class Collision_Line
{
private:
	std::vector<std::pair<D3DXVECTOR2, D3DXVECTOR2>> m_lineVertex;

public:
	void Clear() { m_lineVertex.clear(); }
	void InsertVertex(const D3DXVECTOR2& v1, const D3DXVECTOR2& v2) { m_lineVertex.push_back(std::make_pair(v1, v2)); }
	const std::vector<std::pair<D3DXVECTOR2, D3DXVECTOR2>>& GetVertex() { return m_lineVertex; }

public:
	Collision_Line() {}
	~Collision_Line() {}
};


class Collision_box
{
private:
	D3DXVECTOR2 m_position;
	D3DXVECTOR2 m_size;

public:
	void		SetPosition(const D3DXVECTOR2& position)	{ m_position = position; }
	void		SetSize(const D3DXVECTOR2& size)			{ m_size = size; }
	D3DXVECTOR2	GetPosition()	const { return m_position; }
	D3DXVECTOR2	GetSize()		const { return m_size; }

public:
	bool CalculateCollision(const Collision_box& anotherAABB);
	void RenderBox(LPDIRECT3DDEVICE9 device);

public:
	Collision_box();
	~Collision_box();
};
