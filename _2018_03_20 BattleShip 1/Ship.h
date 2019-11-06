#pragma once
class Ship
{
private:
	const UINT m_size;
	UINT m_life;
	POINT m_modelPos;
	bool m_lookAtRight;

	EasyASE* m_ase;

public:
	UINT GetSize() { return m_size; }
	UINT GetLife() { return m_life; }
	POINT GetPos() { return m_modelPos; }

public:
	void Assign(const POINT& tile, const bool& lookAtRight);
	bool Hit();
	void Attack(const POINT& target);
	void Explosion();

	void Render(const D3DXMATRIX& worldM);

public:
	Ship(const UINT& size);
	~Ship();
};

