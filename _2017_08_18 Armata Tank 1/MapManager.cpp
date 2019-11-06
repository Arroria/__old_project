#include "MainHeader.h"
#include "MapManager.h"

#include "ASE.h"
#include "Map.h"
#include "PlayerManager.h"
#include "EnemyManager.h"

namespace AF
{
	MapManager::MapManager(EnemyManager** enMgr, PlayerManager** plMgr, ItemManager** itMgr)
		: m_enMgr(enMgr)
		, m_plMgr(plMgr)
		, m_itMgr(itMgr)
	{
		m_mapData.insert(std::make_pair( L"Snowland_1", new Map(this, 1, 30) ));
		m_mapData.insert(std::make_pair( L"Snowland_2", new Map(this, 2, 30) ));
		m_mapData.insert(std::make_pair( L"Snowland_3", new Map(this, 3, 30) ));
	}
	MapManager::~MapManager()
	{
		for (auto& iter : m_mapData)
			SAFE_DELETE(iter.second);
	}	



	void MapManager::Update()
	{
		if (m_map)
		{
			m_map->Update();
			if ((*m_enMgr)->GetEnemyCount() == 0 && !m_map->CanIMakeEnemy())
			{
				//GameEnd
				(*m_plMgr)->DestroyPlayer();
				OffMap();
			}
		}
	}

	void MapManager::Render()
	{
		if (m_map)
			m_map->Render();
	}



	void MapManager::SetMap(const std::wstring mapCode)
	{
		m_map = m_mapData[mapCode];
		m_map->Initialize();
	}

	void MapManager::OffMap()
	{
		m_map->Release();
		m_map = nullptr;
	}

	MapTile* MapManager::CheckCrashWithMap(const int& type, const D3DXVECTOR3& crashObject, const float crashRange)
	{
		return m_map ? m_map->GetCrashTile(type,crashObject,crashRange) : nullptr;
	}

	bool MapManager::OutOfMap(const D3DXVECTOR3 & crashObject, const float crashRange)
	{
		return m_map->OutOfMap(crashObject, crashRange);
	}
	void MapManager::CreatePlayer(const D3DXVECTOR3 & playerPos)
	{
		(*m_plMgr)->CreatePlayer(playerPos);
	}
	void MapManager::RegenPlayer()
	{
		if (m_map)	m_map->CreatePlayer();
	}
}
