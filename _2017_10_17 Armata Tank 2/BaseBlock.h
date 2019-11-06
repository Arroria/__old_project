#pragma once

#define BaseBlockScale (1.f / 160 * 100)

class BaseBlock
{
private:
	bool m_destroyThis;
public:
	void DestroyThis() { m_destroyThis = true; }
	bool CheckDestroyThis() { return m_destroyThis; }

protected:
	int m_blockId;

	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_scale;

public:
	D3DXVECTOR3 GetPosition() { return m_position; }
	void		SetPosition(const D3DXVECTOR3& position) { m_position = position; }
	D3DXVECTOR3 GetScale() { return m_scale; }
	void		SetScale(const D3DXVECTOR3& scale) { m_scale = scale; }
	
//Armour
protected:
	bool m_unbreakable;
	float m_maxDurability;
	float m_durability;
	float m_armour;

public:
	float GetDurability() { return m_durability; }
	float GetMaxDurability() { return m_maxDurability; }

	virtual void Hit(const float& firePower, const float& penetration);

//Collision
protected:
	D3DXVECTOR2 m_collisionSize;
	bool m_canPenetrateForMove;
	bool m_canPenetrateForAttack;

public:
	D3DXVECTOR2 GetCollisionSize() { return m_collisionSize; }
	bool IsCanPenetrateForMove() { return m_canPenetrateForMove; }
	bool IsCanPenetrateForAttack() { return m_canPenetrateForAttack; }

public:
	virtual bool Update() PURE;
	virtual bool Render() PURE;

public:
	BaseBlock(const int& blockId, const D3DXVECTOR3& position, const D3DXVECTOR3& scale, const D3DXVECTOR2& collisionSize, const float& durability, const float& armour);
	virtual ~BaseBlock();
};

