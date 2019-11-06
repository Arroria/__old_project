#pragma once
namespace AF
{
	class ASEModel;
	class Map;
	class MapTile;
	class MapObject;

	class EnemyManager;
	class PlayerManager;
	class ItemManager;

	class MapManager
	{
	private:
		std::map<std::wstring, ASEModel*>	m_mapModel;
		std::map<std::wstring, Map*>	m_mapData;


	private:
		Map*	m_map;

		EnemyManager**	m_enMgr;
		PlayerManager**	m_plMgr;
		ItemManager**	m_itMgr;

	public:
		void	SetMap(const std::wstring mapCode);
		void	OffMap();

		MapTile*	CheckCrashWithMap		(const int& type, const D3DXVECTOR3& crashObject, const float crashRange);
		bool		OutOfMap				(const D3DXVECTOR3& crashObject, const float crashRange);
		bool		CheckThereCanItemDrop(const int& x, const int& y);

		void		CreatePlayer(const D3DXVECTOR3& playerPos);
		void		RegenPlayer();
		

		Map*			GetMap()	{ return m_map; }
		EnemyManager*	GetEmgr()	{ return *m_enMgr; }
		PlayerManager*	GetPMger()	{ return *m_plMgr; }
		ItemManager*	GetItMgr()	{ return *m_itMgr; }


	public:
		void	Update();
		void	Render();

	public:
		MapManager(EnemyManager** enMgr, PlayerManager** plMgr, ItemManager** itMgr);
		~MapManager();
	};

}
