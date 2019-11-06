#pragma once
namespace AF
{
	class ASEModel;
	class MapTile;

	class MapManager;

	class Map
	{
	private:
		const	int	m_type;

	private:
		MapManager*	m_mapMgr;

		ASEModel*	m_mapMainModel;

		ASEModel*	m_baseModel;
		ASEModel*	m_blockModel;
		ASEModel*	m_specialModel;
		ASEModel*	m_trunalModel;
		ASEModel*	m_mirrorModel;
		
		int			m_mapEnemyCount;
		int			m_nowCanMakeEnemyCount;

	private:
		MapTile*	m_mapTile[16][16];

	public:
		bool		OutOfMap	(const D3DXVECTOR3& crashObject, const float crashRange);
		MapTile*	GetCrashTile(const int& type, const D3DXVECTOR3& crashObject, const float crashRange);
		void		CreatePlayer();


		bool		CanIMakeEnemy()		{ return m_nowCanMakeEnemyCount > 0; }
		void		IWillMakeEnemy()	{ m_nowCanMakeEnemyCount--; }

	public:
		void	Initialize	();
		void	Update		();
		void	Render		();
		void	Release		();

	public:
		Map(MapManager*	mapMgr, const int& type, const int& mapEnemyCount);
		~Map();
	};



	class MapTile
	{
	protected:
		MapManager*	m_mapMgr;


		int			m_type;
		ASEModel*	m_tileModel;
		D3DXVECTOR3	m_tile3DWorldPos;

		bool	m_unbreakable;
		int		m_health;

		bool	m_isDisabled;
		bool	m_isLookRight;

		
	//just block
	protected:
		int		m_blockHealth[2][2];
		int		m_crashedBlockIs;


	public:
		bool	CheckCrash(const D3DXVECTOR3& crashObject, const float crashRange);
		void	CrashDamage(const int& damage);
		bool	IsLookRight	()	{ return m_isLookRight; }

	public:
		int		GetType()		{ return m_type; }
		bool	IsActive()		{ return !m_isDisabled; }
		void	SetDestroy()	{ m_isDisabled = true; }

	public:
		void	Initialize();
		void	Update();
		void	Render();
		void	Release();

	public:
		MapTile(MapManager*	mapMgr, const int& type, ASEModel* tileModel, const D3DXVECTOR3& tile3DWorldPos, const bool& isLookRight = false);
		virtual ~MapTile();
	};
}
