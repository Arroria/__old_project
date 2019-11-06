#pragma once
#include "Player.h"

class Frame;
class Ship;

class Enemy;
class Bullet;

class PlayerShip
	: public Player
{
private:
	ASE* m_model;
	ASE* m_turretModel;
	Ship* m_ship;

	D3DXQUATERNION m_turretRotation;

public:
	Frame* GetFrame() override;

private:
	void ControlFrame();
	void ControlCamera();
	void Attack(std::vector<Bullet*>* bulletContainer);
	void TurretRotation();

public:
	void Initialize() override;
	void Update(std::vector<Enemy*>* enemyJet, std::vector<Bullet*>* bulletContainer) override;
	void Render() override;
	void Release() override;

public:
	PlayerShip();
	~PlayerShip();
};
