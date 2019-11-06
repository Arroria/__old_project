#pragma once
class Ship
{
private:
	UINT m_size;
	UINT m_life;

	D3DXVECTOR3 m_pos;
	POINT m_tilePos;
	bool m_lookAtY;

	UINT m_frame;

public:
	UINT GetSize() { return m_size; }
	UINT GetLife() { return m_life; }
	POINT GetTilePos() { return m_tilePos; }
	D3DXVECTOR3 GetPos() { return m_pos; }

	void SetPlaceData(const POINT& tilePos, const bool& isLookAtY);

	void Hit() { if (m_life) m_life--; }

public:
	void Update(const D3DXVECTOR3& pos);
	void Render();

public:
	Ship(const UINT& size);
	~Ship();
};

