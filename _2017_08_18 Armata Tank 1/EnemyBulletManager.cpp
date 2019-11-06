#include "MainHeader.h"
#include "EnemyBulletManager.h"
#include "MapManager.h"

#include "EnemyBullet.h"
#include "ASE.h"
#include "PlayerManager.h"

namespace AF
{
	EnemyBulletManager::EnemyBulletManager(AF::MapManager ** mapManager, PlayerManager** playerManager)
		: m_mapManager(mapManager)
		, m_playerManager(playerManager)
	{
		ASELoader aseLoader;

		m_enemyBulletModel.insert(std::make_pair(L"TEST", aseLoader.LoadAseFile(L"./Resource/Tank/Tank_Body.ase")));
	}
	EnemyBulletManager::~EnemyBulletManager()
	{
		for (auto& iter : m_enemyBullet)
				SAFE_DELETE(iter);
	}



	void EnemyBulletManager::Update()
	{
		auto& iter = m_enemyBullet.begin();
		while (iter != m_enemyBullet.end())
		{
			(*iter)->Update();
			if ((*iter)->GetDelete())
			{
				SAFE_DELETE(*iter);
				iter = m_enemyBullet.erase(iter);
			}
			else
				iter++;
		}
	}

	void EnemyBulletManager::Render()
	{
		for (auto& iter : m_enemyBullet)
			iter->Render();
	}



	EnemyBullet * EnemyBulletManager::CreateEnemyBullet(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & direction)
	{
		EnemyBullet* temp = new EnemyBullet(this, m_enemyBulletModel[L"TEST"]);
		temp->SetPosition(pos);
		temp->SetDirection(direction);
		m_enemyBullet.push_back(temp);
		return temp;
	}

	bool EnemyBulletManager::IsCrashedByWall(const D3DXVECTOR3 & pos)
	{
		return (*m_mapManager)->OutOfMap(pos, 3);
	}

	MapTile * EnemyBulletManager::CheckCrashMapTile(const D3DXVECTOR3 & pos, const int & tile)
	{
		return (*m_mapManager)->CheckCrashWithMap(tile, pos, 3);
	}

	Player * EnemyBulletManager::CheckCrashPlayer(const D3DXVECTOR3 & pos)
	{
		if ((*m_playerManager)->CheckCrashPlayerPlease(pos, 3))
			return (*m_playerManager)->GetPlayer();
		return nullptr;
	}
}
