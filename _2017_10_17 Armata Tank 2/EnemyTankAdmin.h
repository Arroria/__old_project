#pragma once
class BaseTank;
class BaseShell;

class PlayerAdmin;
class ShellAdmin;
class Map;
class Item;

class EnemyTankAdmin
{
private:
	PlayerAdmin** m_playerAdmin;
	ShellAdmin** m_shellAdmin;
	Map** m_map;

	std::list<Item*>* m_itemContainer;

private:
	std::list<BaseTank*> m_enemyTank;

public:
	std::list<BaseTank*>* GetEnemyContainer() { return &m_enemyTank; }

public:
	bool CreateShell(BaseShell* shell);

public:
	bool RegisterEnemyTank(BaseTank* enemy);

	bool EnemyUpdate();
	bool EnemyRender();

public:
	EnemyTankAdmin(PlayerAdmin** playerAdmin, ShellAdmin** shellAdmin, Map** map, std::list<Item*>* itemContainer);
	~EnemyTankAdmin();
};

