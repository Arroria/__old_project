#pragma once

class Ship;
typedef std::pair<Ship*, bool> Tile;

class Map
{
private:
	std::array<Tile, 100> m_tile;
	D3DXVECTOR3 m_pos;

public:
	static UINT PointToLine	(const POINT& pos)				{ return pos.x + pos.y * 10; }
	static UINT PointToLine	(const UINT& x, const UINT& y)	{ return x + y * 10; }
	static POINT LineToPoint	(const UINT& pos)			{ POINT point; point.x = pos % 10; point.y = pos / 10; return point; }
	
	Tile& GetTile	(const POINT& pos)					{ return GetTile(PointToLine(pos)); }
	Tile& GetTile	(const UINT& x, const UINT& y)		{ return GetTile(PointToLine(x, y)); }
	Tile& GetTile	(const UINT& pos)					{ return m_tile[pos]; }

	 
	D3DXVECTOR3	PointToVec	(const POINT& pos)				{ return PointToVec(pos.x, pos.y); }
	D3DXVECTOR3	PointToVec	(const UINT& x, const UINT& y)	{ return D3DXVECTOR3(x - 4.5f, 0, y - 4.5f) + m_pos; }
	D3DXVECTOR3	LineToVec	(const UINT& pos)				{ return PointToVec(LineToPoint(pos)); }

public:
	bool GetMousePos(POINT& out);


public:
	void ClearMap();
	bool RegistShip(Ship* ship, const bool& lookAtY, const POINT& pos, const bool& onlyCheck);
	void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }

public:
	void RenderMap	();
	void RenderTile	(const UINT& pos, Texture* tex);

public:
	Map();
	~Map();
};

