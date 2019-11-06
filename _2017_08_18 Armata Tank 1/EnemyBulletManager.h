#pragma once

namespace AF
{
	class EnemyBullet;
	class ASEModel;
	class MapTile;
	class MapManager;
	class Player;
	class PlayerManager;
	class EnemyBulletManager
	{
	private:
		AF::MapManager**	m_mapManager;
		AF::PlayerManager**	m_playerManager;

	private:
		std::list<EnemyBullet*>				m_enemyBullet;
		std::map<std::wstring, ASEModel*>	m_enemyBulletModel;

	public:
		EnemyBullet*	CreateEnemyBullet(const D3DXVECTOR3& pos, const D3DXVECTOR3& direction);
		bool			IsCrashedByWall(const D3DXVECTOR3& pos);


	public:
		MapTile*		CheckCrashMapTile(const D3DXVECTOR3& pos, const int& tile);
		Player*			CheckCrashPlayer(const D3DXVECTOR3& pos);


	public:
		void	Render();
		void	Update();


	public:
		EnemyBulletManager(AF::MapManager** mapManager, PlayerManager**	playerManager);
		~EnemyBulletManager();
	};
}
