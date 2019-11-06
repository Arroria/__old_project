#pragma once
#include "Boss.h"

class EnemyAttack;
class Yamato :
	public Boss
{
private:
	ASE* m_model;
	ASE* m_turretModel;

	bool m_isDead;

public:
	bool IsDead() override { return m_isDead; }

public:
	void Initialize() override;
	void Update(std::vector<EnemyAttack*>* attackContainer, const D3DXVECTOR3& playerPos) override;
	void Render() override;
	void Release() override;

public:
	Yamato();
	~Yamato();
};

