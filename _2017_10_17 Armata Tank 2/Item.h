#pragma once

class BaseTank;
class ASE;
class Item
{
private:
	bool m_destroyThis;
public:
	bool CheckDestroyThis() { return m_destroyThis; }
	void DestroyThis() { m_destroyThis = true; }

private:
	//Item Identity
	int m_itemCode;
	Texture* m_txItem;
	ASE* m_aseItem;

	//Transform
	D3DXVECTOR3 m_position;
	float m_rotation;
	D3DXVECTOR3 m_scale;

	//Release
	double m_lostTime;

	
	//Collision
	D3DXVECTOR2 m_collisionSize;


public:
	//Transform
	D3DXVECTOR3 GetPosition() { return m_position; }

	//Collision
	D3DXVECTOR2 GetCollisionSize() { return m_collisionSize; }

public:
	void EffectForPlayer(BaseTank* target);

public:
	void Update();
	void Render();

public:
	Item(const int& itemCode, const D3DXVECTOR3& position);
	~Item();
};

