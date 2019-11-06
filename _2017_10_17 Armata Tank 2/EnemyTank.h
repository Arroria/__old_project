#pragma once
#include "BaseTank.h"

class EnemyTankAdmin;

enum class EnemyTankType
{
	Cannon,
	AutoCannon,
	Rocket,
};

class EnemyTank :
	public BaseTank
{
private:
	EnemyTankAdmin* m_enemyTankAdmin;
public:
	void SetEnemyTankAdmin(EnemyTankAdmin* enemyTankAdmin) { m_enemyTankAdmin = enemyTankAdmin; }

private:
	//Tank Move
	Texture* m_txWheel;
	Texture* m_txBody;
	D3DXVECTOR3 m_bodyObjectiveDirection;
	D3DXVECTOR3 m_bodyLookingDirection;
	float m_bodyLookingRotation;

	//Tank Aiming
	D3DXVECTOR3 m_weaponObjectiveDirection;
	D3DXVECTOR3 m_weaponLookingDirection;
	float m_weaponLookingRotation;

	//Tank Attack
	Texture* m_txWeapon;
	EnemyTankType m_playerWeaponType;
	double m_fireCooltime;


	bool m_needTurn;
	
public:
	void SetTurn() { m_needTurn = true; }

public:
	virtual void Move() override;
	virtual void Aiming() override;
	virtual void Attack() override;

	virtual bool Render() override;

public:
	EnemyTank(const D3DXVECTOR3& position, const EnemyTankType& tankType);
	virtual ~EnemyTank();
};

