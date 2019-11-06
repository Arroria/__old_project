#pragma once

class Frame;
class Bullet;
class EnemyAttack;

class PlayerCursor;

class Enemy
{
protected:
	UINT m_health;

	std::vector<Enemy**> m_myReferencer;
	PlayerCursor* m_hitEffecter;

public:
	virtual Frame* GetFrame() PURE;

	void Hit(const UINT& damage = 1);
	void SetHealth(const UINT& health)	{ m_health = health; }
	UINT GetHealth()					{ return m_health; }

	void AddReferencer(Enemy** referencer) { m_myReferencer.push_back(referencer); }
	void SetHitEffecter(PlayerCursor* hitEffecter) { m_hitEffecter = hitEffecter; }

public:
	virtual void Initialize(const D3DXVECTOR3& pos) PURE;
	virtual void Update(std::vector<EnemyAttack*>* attackContainer, const D3DXVECTOR3& playerPos) PURE;
	virtual void Render() PURE;
	virtual void Release() PURE;

public:
	Enemy();
	~Enemy();
};

