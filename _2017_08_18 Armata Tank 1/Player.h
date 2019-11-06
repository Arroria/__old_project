#pragma once
#include "BaseObject.h"

namespace AF
{
	class ASEModel;
	class InputManager;

	class PlayerManager;
	struct PlayerData;

	class Player :
		public AF::BaseObject
	{
	public:
		struct Player3DModel
		{
			ASEModel*	chassis;
			ASEModel*	turret;
			ASEModel*	gun;
		};
		struct PlayerInfo
		{

		};

	private:
		AF::PlayerManager*	m_playerManager;

		Player3DModel	m_playerModel;
		PlayerInfo		m_playerInfo;


		int	m_health;
		int	m_shield;


	private:
		float		m_objectiveMoveDirection;
		float		m_turretDirection;

		D3DXMATRIX	m_chassisTransformMatrix;
		D3DXMATRIX	m_TurretTransformMatrix;

	public:
		void	MoveChassis();
		void	RotationTurret();
		void	Fire();
		


	public:
		void	GiveDamage(const int& damage);
		int	GetHealth()		{ return m_health; }


	public:
		virtual	void	Initialize()	override;
		virtual	void	Update()		override;
		virtual	void	Render()		override;
		virtual	void	Release()		override;


	public:
		Player(AF::PlayerManager* playerManager, Player3DModel playerModel, PlayerInfo playerInfo);
		virtual ~Player();
	};

}
