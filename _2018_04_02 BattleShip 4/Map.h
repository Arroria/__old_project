#pragma once

class Ship;

typedef std::pair<Ship*, bool> Tile;
class Map
{
private:
	std::array<Tile, 100> m_tile;
	D3DXVECTOR3 m_pos;


public:
	static	POINT	LineToPoint(const UINT& pos)				{ POINT point; point.x = pos % 10; point.y = pos / 10; return point; }
	static	UINT	PointToLine(const POINT& pos)				{ return pos.x + pos.y * 10; }
	static	UINT 	PointToLine(const UINT& x, const UINT& y)	{ return x + y * 10; }

	D3DXVECTOR3 LineToVec	(const UINT& pos)				const { return PointToVec(LineToPoint(pos)); }
	D3DXVECTOR3 PointToVec	(const POINT& pos)				const { return PointToVec(pos.x, pos.y); }
	D3DXVECTOR3 PointToVec	(const UINT& x, const UINT& y)	const { return D3DXVECTOR3(x - 4.5f, 0, y - 4.5f) + m_pos; }

	Tile& GetTile(const UINT& pos)				{ return m_tile[pos]; }
	Tile& GetTile(const POINT& pos)				{ return GetTile(PointToLine(pos)); }
	Tile& GetTile(const UINT& x, const UINT& y)	{ return GetTile(PointToLine(x, y)); }

public:
	void MapClear();

	void RenderMap(const bool& myTurn, Texture* tex);
	void RenderTile(const POINT& tilePos, Texture* tex);

	bool GetCursor(POINT& out);


	void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos() { return m_pos; }

public:
	Map();
	~Map();
};

