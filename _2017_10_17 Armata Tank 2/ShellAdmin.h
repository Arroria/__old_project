#pragma once
class BaseShell;
class BaseTank; //temp

class PlayerAdmin;
class EnemyTankAdmin;
class Map;

class ShellAdmin
{
private:
	PlayerAdmin** m_playerAdmin;
	EnemyTankAdmin** m_enemyAdmin;
	Map** m_map;

private:
	std::list<BaseShell*> m_playerShell;
	std::list<BaseShell*> m_enemyShell;

public:
	void RegisterPlayerShell(BaseShell* shell);
	void RegisterEnemyShell(BaseShell* shell);

public:
	bool ShellUpdate();
	bool ShellInterAction();
	bool ShellRender();

private:
	void ShellDestroy();

public:
	ShellAdmin(PlayerAdmin** playerAdmin, EnemyTankAdmin** enemyAdmin, Map** map);
	~ShellAdmin();
};

