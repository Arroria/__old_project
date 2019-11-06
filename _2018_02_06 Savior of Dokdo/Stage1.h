#pragma once

class Player;
class Enemy;
class Boss;

class Bullet;
class EnemyAttack;

class Effect3D;

class PlayerCursor;

class Stage1
{
private:
	std::map<std::wstring, ASE*> m_aseList;

	Player* m_player;
	PlayerCursor* m_cursor;
	
	ASE* m_skyBox;
	ASE* m_seaModel;

	Boss* m_boss;
	std::vector<Enemy*> m_enemy;
	std::vector<Bullet*> m_bulletContainer;
	std::vector<EnemyAttack*> m_enemyAttack;

	std::vector<Effect3D*> m_3dEffect;

	LPD3DXFONT m_font;

private:
	void CreateEnemy();

public:
	void Initialize();
	void Update();
	void Render();
	void Release();

public:
	Stage1();
	~Stage1();
};

