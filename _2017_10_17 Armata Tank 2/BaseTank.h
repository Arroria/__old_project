#pragma once

#define BaseTankScale (1.f / 64 * 100)

class BaseTank
{
private:
	bool m_destroyThis;
public:
	void DestroyThis() { m_destroyThis = true; }
	bool CheckDestroyThis() { return m_destroyThis; }

protected:
	D3DXVECTOR3 m_position;
	float m_rotation;
	D3DXVECTOR3 m_scale;

	float m_maxDurability;
	float m_durability;
	float m_armour;

public:
	D3DXVECTOR3 GetPosition() { return m_position; }
	void SetPosition(const D3DXVECTOR3& position) { m_position = position; }

protected:
	D3DXVECTOR2 m_collisionSize;
public:
	D3DXVECTOR2 GetCollisionSize() { return m_collisionSize; }

protected:
	virtual void Move() PURE;
	virtual void Aiming() PURE;
	virtual void Attack() PURE;

public:
	virtual void Hit(const float& firePower, const float& penetration);

public:
	virtual bool Update();
	virtual bool Render() PURE;
	
public:
	BaseTank(const D3DXVECTOR3& position);
	BaseTank(const D3DXVECTOR3& position, const float& durability, const float& armour);
	virtual ~BaseTank();


	//구조 ㅈ되서 임시로 여기에 만듬
public:
	virtual void Repair(const int& repairAmount);

};

