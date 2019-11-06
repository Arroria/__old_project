#pragma once
#include "BaseObject.h"
namespace AF
{
	class EnemyBulletManager;
	class ASEModel;
	class MapTile;

	class EnemyBullet :
		public AF::BaseObject
	{
	private:
		EnemyBulletManager*	m_enemyBulletManager;

		ASEModel*	m_bulletModel;

		bool	m_delete;

		MapTile* m_crashedTile;

	public:
		bool	GetDelete() { return m_delete; }

	public:
		void	Initialize();
		void	Update();
		void	Render();
		void	Release();

	public:
		EnemyBullet(EnemyBulletManager* enemyBulletManager, ASEModel* bulletModel);
		~EnemyBullet();
	};
}


