#pragma once
#include "Player.h"

class Jet;
class Enemy;
class EnemyJet;

class Bullet;

class PlayerJet
	: public Player
{
private:
	ASE* m_model;
	Jet* m_jet;

public:
	Frame* GetFrame() override;

private:
	void ControlAirframe();
	void ControlCamera();
	void Attack(std::vector<Bullet*>* bulletContainer);
	
public:
	void Initialize() override;
	void Update(std::vector<Enemy*>* enemyJet, std::vector<Bullet*>* bulletContainer) override;
	void Render() override;
	void Release() override;

public:
	PlayerJet();
	~PlayerJet();
};

