#pragma once
#include "Lockoner.h"


class Frame;

class Enemy;
class EnemyJet;
class Bullet;

class Player
{
protected:
	Lockoner m_lockoner;
	UINT m_attackCooltime;
	UINT m_health;

	bool m_isFired;
	D3DXVECTOR3 m_ammoFirePos;

	UINT m_cameraShake;

public:
	virtual Frame* GetFrame() PURE;

	Lockoner* GetLockoner() { return &m_lockoner; }

	void Hit(const UINT& damage = 1)	{ if (m_health > damage) m_health -= damage; else m_health = 0; m_cameraShake = 15; }
	void SetHealth(const UINT& health)	{ m_health = health; }
	UINT GetHealth()					{ return m_health; }

	bool IsFired() { return m_isFired; }
	D3DXVECTOR3 GetAmmoFirePosition() { return m_ammoFirePos; }

public:
	virtual void Initialize() PURE;
	virtual void Update(std::vector<Enemy*>* enemyJet, std::vector<Bullet*>* bulletContainer) PURE;
	virtual void Render() PURE;
	virtual void Release() PURE;

public:
	Player();
	~Player();
};

