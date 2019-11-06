#include "MainHeader.h"
#include "EnemyManager.h"

#include "ASE.h"

#include "EnemyBulletManager.h"
#include "MapManager.h"
#include "Enemy.h"
#include "ItemManager.h"

namespace AF
{
	EnemyManager::EnemyManager(MapManager** mapManager, EnemyBulletManager** enemyBulletManager, ItemManager** itemManager)
		: m_mapManager(mapManager)
		, m_enemyBulletManager(enemyBulletManager)
		, m_itemManager(itemManager)
	{
		ASELoader aseLoader;
		m_enemyModel.insert(std::make_pair( L"Body1", aseLoader.LoadAseFile(L"./Resource/Tank/Enemy0_Body.ASE") ));
		m_enemyModel.insert(std::make_pair( L"Turret1", aseLoader.LoadAseFile(L"./Resource/Tank/Enemy0_Turret.ASE") ));
	}
	EnemyManager::~EnemyManager()
	{
		for (auto& iter : m_enemy)
			SAFE_DELETE(iter);
	}



	void EnemyManager::Update()
	{
		auto& iter = m_enemy.begin();
		while (iter != m_enemy.end())
		{
			(*iter)->Update();
			if ((*iter)->µÚÁ®¶ù())
			{
				SAFE_DELETE(*iter);
				iter = m_enemy.erase(iter);
			}
			else
				iter++;
		}
	}
	
	void EnemyManager::Render()
	{
		for (auto& iter : m_enemy)
			iter->Render();
	}
	
	
	
	Enemy * EnemyManager::CreateEnemy()
	{
		Enemy* temp = new Enemy(this, m_enemyModel[L"Body1"], m_enemyModel[L"Turret1"]);
		m_enemy.push_back(temp);
		return temp;
	}

	bool EnemyManager::CheckCrashedMap(const D3DXVECTOR3 & nextPos)
	{
		if ((*m_mapManager)->CheckCrashWithMap(1, nextPos, 20) ||
			(*m_mapManager)->CheckCrashWithMap(2, nextPos, 20) ||
			(*m_mapManager)->CheckCrashWithMap(3, nextPos, 20) ||
			(*m_mapManager)->CheckCrashWithMap(4, nextPos, 20) ||
			(*m_mapManager)->CheckCrashWithMap(5, nextPos, 20) ||
			(*m_mapManager)->CheckCrashWithMap(7, nextPos, 20) ||
			(*m_mapManager)->OutOfMap(nextPos, 20))
			return true;
		return false;
	}
	Enemy * EnemyManager::CrashWithBullet(const D3DXVECTOR3 & bulletPos, const int & bulletSize)
	{
		for (auto& iter : m_enemy)
		{
			D3DXVECTOR3 enemyPos = iter->GetPosition();
			if (enemyPos.x + 20 > bulletPos.x - bulletSize &&
				enemyPos.x - 20 < bulletPos.x + bulletSize &&
				enemyPos.z - 20 < bulletPos.z + bulletSize &&
				enemyPos.z + 20 > bulletPos.z - bulletSize)
				return iter;
		}
		return nullptr;
	}
	void EnemyManager::CreateItem(const D3DXVECTOR3 & itemPos, const int & itemType)
	{
		(*m_itemManager)->CreateItem(itemType, itemPos);
	}
}
