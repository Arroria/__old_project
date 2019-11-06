#pragma once

class Ship;
class Map
{
private:
	std::array<std::pair<Ship*, bool>, 100> m_map;
	D3DXVECTOR3 m_pos;

public:
	void RegistShip(const UINT& x, const UINT& y, Ship* shipPtr);

	/* Tile Support */
	std::pair<Ship*, bool>& GetTile(const UINT& x, const UINT& y) { return m_map[x + y * 10]; }
	Ship*	GetTileState(const UINT& x, const UINT& y)	{ return GetTile(x, y).first; }
	bool	IsBombed	(const UINT& x, const UINT& y)	{ return GetTile(x, y).second; }

	void Explosion(const UINT& x, const UINT& y);


	bool GetCursorTile(POINT& pos);

	D3DXVECTOR3 GetMapPos() { return m_pos; }

public:
	void Render(const bool& viewCursor);

	void DrawTile(const UINT& x, const UINT& y, const D3DXVECTOR3& offset, Texture* tex);
	
public:
	Map(const D3DXVECTOR3& pos);
	~Map();
};

