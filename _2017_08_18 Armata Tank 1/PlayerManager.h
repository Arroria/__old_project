#pragma once
namespace AF
{
	class ASEModel;
	class InputManager;

	class InputManager;
	class PlayerAttackManager;
	class EnemyManager;
	class EnemyBulletManager;
	class MapManager;
	class TextureManager;
	class ItemManager;

	class Player;
	struct Player3DModel;
	struct PlayerInfo;

	class PlayerManager
	{
	private:
		TextureManager**		m_textureManager;
		InputManager**			m_playerManager;
		PlayerAttackManager**	m_playerAttackManager;
		EnemyManager**			m_enemyManager;
		EnemyBulletManager**	m_enemyBulletManager;
		MapManager**			m_mapManager;
		ItemManager**			m_itemManager;

	public:
		InputManager**			GetInputManager()		{ return m_playerManager;		}
		PlayerAttackManager**	GetAttackManager()		{ return m_playerAttackManager;	}
		EnemyManager**			GetEnemyManager()		{ return m_enemyManager;		}
		EnemyBulletManager**	GetEnemyBulletManager()	{ return m_enemyBulletManager;	}
		MapManager**			GetMapManager()			{ return m_mapManager;			}
		ItemManager**			GetItemManager()		{ return m_itemManager;			}
		


	private:
		std::map<std::wstring, ASEModel*>	m_playerModel;

		Player*	m_player;
		int		m_playerLife;

	public:
		void	CreatePlayer(const D3DXVECTOR3& pos);
		void	DestroyPlayer();

		bool	CheckCrashPlayerPlease(const D3DXVECTOR3& pos, const int& size);
		Player*	GetPlayer() { return m_player; }

	public:
		bool	CheckCrashedMap(const D3DXVECTOR3& nextPos);



	public:
		void	Initialize();
		void	Update();
		void	Render();
		void	RenderUI();



	public:
		PlayerManager(InputManager** inputManager, PlayerAttackManager** playerAttackManager, EnemyManager** enemyManager, EnemyBulletManager** enemyBulletManager, MapManager** mapManager, TextureManager** textureManager, ItemManager** itemManager);
		~PlayerManager();
	};
}

