#pragma once
namespace AF
{
	enum BulletType;

	class MapManager;
	class EnemyManager;

	class PlayerBullet;
	class ASEModel;
	class MapTile;
	class Enemy;

	class PlayerAttackManager
	{
	private:
		AF::MapManager**	m_mapManager;
		AF::EnemyManager**	m_enemyManager;

	private:
		std::list<PlayerBullet*>			m_playerBullet;
		std::map<std::wstring, ASEModel*>	m_playerBulletModel;

	public:
		PlayerBullet*	CreatePlayerBullet	(const BulletType& type, const D3DXVECTOR3& pos, const D3DXVECTOR3& direction);
		bool			IsCrashedByWall		(const D3DXVECTOR3& pos);


	public:
		MapTile*		CheckCrashMapTile	(const D3DXVECTOR3& pos, const int& tile);
		Enemy*			CheckCrashEnemy		(const D3DXVECTOR3& pos);


	public:
		void	Render();
		void	Update();


	public:
		PlayerAttackManager(AF::MapManager** mapManager, EnemyManager**	enemyManager);
		~PlayerAttackManager();
	};
}
