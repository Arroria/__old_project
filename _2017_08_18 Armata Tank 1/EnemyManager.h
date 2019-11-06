#pragma once
namespace AF
{
	class ASEModel;

	class MapManager;
	class EnemyBulletManager;
	class ItemManager;

	class Enemy;
	class EnemyManager
	{
	private:
		std::map<std::wstring, ASEModel*>	m_enemyModel;
		std::list<Enemy*>					m_enemy;


	private:
		MapManager**	m_mapManager;
		EnemyBulletManager**	m_enemyBulletManager;
		ItemManager** m_itemManager;

	public:
		EnemyBulletManager**	GetBltMger()	{ return m_enemyBulletManager; }

	public:
		Enemy*	CreateEnemy();


	public:
		bool	CheckCrashedMap(const D3DXVECTOR3& nextPos);
		Enemy*	CrashWithBullet(const D3DXVECTOR3& bulletPos, const int& bulletSize);
		void	CreateItem(const D3DXVECTOR3&itemPos, const int& itemType);
		int		GetEnemyCount() { return m_enemy.size(); }


	public:
		void	Update();
		void	Render();

	public:
		EnemyManager(MapManager** mapManager, EnemyBulletManager**	enemyBulletManager, ItemManager** itemManager);
		~EnemyManager();
	};
}

