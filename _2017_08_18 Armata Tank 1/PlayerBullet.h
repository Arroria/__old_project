#pragma once
#include "BaseObject.h"
namespace AF
{
	class PlayerAttackManager;
	class ASEModel;
	class MapTile;


	enum BulletType
	{
		BulletType_Default,
	};


	class PlayerBullet
		: public BaseObject
	{
	private:
		PlayerAttackManager*	m_playerBulletManager;

		ASEModel*	m_bulletModel;

		bool	m_delete;

		MapTile* m_crashedTile;

	private:
		void	CRASHYEAHHA();

	public:
		bool	GetDelete()	{ return m_delete; }

	public:
		void	Initialize();
		void	Update();
		void	Render();
		void	Release();

	public:
		PlayerBullet(PlayerAttackManager* playerBulletManager, ASEModel* bulletModel);
		~PlayerBullet();
	};
}

