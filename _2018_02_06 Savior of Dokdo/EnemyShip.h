#pragma once
#include "Enemy.h"

class Ship;

class EnemyShip :
	public Enemy
{
private:
	ASE* m_model;
	Ship* m_ship;

	bool m_rollToRight;
	UINT m_mT;
	UINT m_mD;


	UINT m_attackCooltime;
	UINT m_attackCount;

	Texture* m_texHealthFrame;

	bool m_type;

public:
	Frame* GetFrame() override;

private:
	void Attack(std::vector<EnemyAttack*>* attackContainer, const D3DXVECTOR3& playerPos);

public:
	void Initialize(const D3DXVECTOR3& pos) override;
	void Update(std::vector<EnemyAttack*>* attackContainer, const D3DXVECTOR3& playerPos) override;
	void Render() override;
	void Release() override;

public:
	EnemyShip(ASE* model, const bool& type);
	~EnemyShip();
};

