#pragma once
#include "BaseObject.h"
namespace AF
{
	class ASEModel;
	class EnemyManager;

	class Enemy :
		public AF::BaseObject
	{
	private:
		AF::EnemyManager*	m_enemyManager;

		ASEModel*	m_chassisModel;
		ASEModel*	m_turretModel;

		D3DXMATRIX	±ÍÂ÷³¶;
		int			³­_ÀÌ¹Ì_ÃÑ¾ËÀ»_½ú¾î;

		int			m_health;
		bool		m_delete;

	private:
		float	m_objectiveMoveDirection;
		bool	m_needTure;

	public:
		void	MoveChassis();
		void	Fire();

		void	GiveDamage(const int& damage);
		bool	µÚÁ®¶ù()	{ return m_delete; }


	public:
		virtual	void	Initialize()	override;
		virtual	void	Update()		override;
		virtual	void	Render()		override;
		virtual	void	Release()		override;


	public:
		Enemy(AF::EnemyManager* enemyManager, ASEModel* bodyModel, ASEModel* turret);
		virtual ~Enemy();
	};

}
