#pragma once

class Ship;
class Map
{
	typedef std::pair<Ship*, bool> Tile;
private:
	std::array<Tile, 100> m_tile;
	D3DXVECTOR3 m_pos;

public:
	/* Suport */
	Tile&	GetTile(const UINT& x, const UINT& y);
	Tile&	GetTile(const POINT& pos);
	Tile&	GetTile(const UINT& pos);
	UINT	PointToLine(const UINT& x, const UINT& y);
	UINT	PointToLine(const POINT& pos);
	POINT	LineToPoint(const UINT& pos);
	D3DXVECTOR3	PointToVector(const UINT& x, const UINT& y);
	D3DXVECTOR3	PointToVector(const POINT& pos);
	D3DXVECTOR3	LineToVector(const UINT& pos);

	/* TileData */
	bool	Hit(const POINT& pos);

public:
	void SetMapPos(const D3DXVECTOR3& pos) { m_pos = pos; }

	void ZeroMap();
	bool RegistShip(Ship* ship, const POINT& pos, const bool& lookAtY, const bool& justCheck = false);                                                                               
	bool RayCastInTile(POINT& pos);

public:
	void Update();
	void RenderMap();
	void RenderTile(const POINT& pos, Texture* tex = nullptr);

public:
	Map();
	~Map();
};

