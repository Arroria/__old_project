#pragma once
class Ship
{
private:
	UINT m_size;
	UINT m_life;

	POINT m_tilePos;
	bool m_lookAtY;

public:
	UINT GetSize()	{ return m_size; }
	UINT GetLife()	{ return m_life; }
	void Hit()		{ m_life--; }
	
	POINT GetTilePos() { return m_tilePos; }
	bool GetLookAtY() { return m_lookAtY; }

public:
	void Placed(const POINT& tilePos, const bool& lookAtY);

	void Render(const D3DXMATRIX& worldM);

public:
	Ship(const UINT& size);
	~Ship();
};

