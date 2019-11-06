#pragma once
#include "BaseTank.h"

enum class PlayerTankWeaponType
{
	Cannon,
	AutoCannon,
	Rocket,
};

class PlayerAdmin;
class PlayerTank :
	public BaseTank
{
private:
	PlayerAdmin* m_playerAdmin;
public:
	void SetPlayerAdmin(PlayerAdmin* playerAdmin) { m_playerAdmin = playerAdmin; }

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
	PlayerTankWeaponType m_playerWeaponType;
	double m_fireCooltime;

	//Unclassified
	Texture* m_txRader;

public:
	bool ChangeWeapon(const PlayerTankWeaponType& weaponType);
	
protected:
	virtual void Move() override;
	virtual void Aiming() override;
	virtual void Attack() override;

public:
	virtual bool Render() override;

public:
	PlayerTank(const D3DXVECTOR3& spawnPoint, const float& durability, const float& armour);
	virtual ~PlayerTank();
};

