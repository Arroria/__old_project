#pragma once
class BaseTank;
class BaseShell;
class Item;

class ShellAdmin;
class EnemyTankAdmin;
class Map;

class PlayerTank;
class PlayerAdmin
{
private:
	ShellAdmin** m_shellAdmin;
	EnemyTankAdmin** m_enemyAdmin;
	Map** m_map;
	std::list<Item*>* m_itemContainer;

private:
	PlayerTank* m_player;

public:
	bool CreateShell(BaseShell* shell);

public:
	BaseTank* GetPlayer();

public:
	bool RegisterPlayer(PlayerTank* player);
	bool PlayerUpdate();
	bool PlayerRender();

public:
	PlayerAdmin(ShellAdmin** shellAdmin, EnemyTankAdmin** enemyAdmin, Map** map, std::list<Item*>* itemContainer);
	~PlayerAdmin();
};

