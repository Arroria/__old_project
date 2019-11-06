#pragma once
class BaseShell
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
	D3DXVECTOR3 m_direction;

	float m_firePower;
	float m_penetration;
	float m_speed;

public:
	D3DXVECTOR3 GetPosition() { return m_position; }

	float GetFirePower() { return m_firePower; }
	float GetPenetration() { return m_penetration; }

protected:
	D3DXVECTOR2 m_collisionSize;
public:
	D3DXVECTOR2 GetCollisionSize() { return m_collisionSize; }

public:
	virtual void Move() PURE;

	virtual bool Update();
	virtual bool Render() PURE;

public:
	BaseShell(const D3DXVECTOR3& position, const D3DXVECTOR3& direction);
	BaseShell(const D3DXVECTOR3& position, const D3DXVECTOR3& direction, const float& firePower, const float& penetration);
	virtual ~BaseShell();
};

