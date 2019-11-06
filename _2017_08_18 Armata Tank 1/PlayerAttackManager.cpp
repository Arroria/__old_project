#include "MainHeader.h"
#include "PlayerAttackManager.h"

#include "ASE.h"
#include "MapManager.h"
#include "EnemyManager.h"

#include "PlayerBullet.h"

namespace AF
{
	PlayerAttackManager::PlayerAttackManager(AF::MapManager ** mapManager, EnemyManager** enemyManager)
		: m_mapManager(mapManager)
		, m_enemyManager(enemyManager)
	{
		ASELoader aseLoader;
		
		m_playerBulletModel.insert(std::make_pair( L"TEST", aseLoader.LoadAseFile(L"./Resource/Tank/Tank_Body.ase") ));
	}
	PlayerAttackManager::~PlayerAttackManager()
	{
	}



	void PlayerAttackManager::Update()
	{
		auto& iter = m_playerBullet.begin();
		while (iter != m_playerBullet.end())
		{
			(*iter)->Update();
			if ((*iter)->GetDelete())
			{
				SAFE_DELETE(*iter);
				iter = m_playerBullet.erase(iter);
			}
			else
				iter++;
		}
	}

	void PlayerAttackManager::Render()
	{
		for (auto& iter : m_playerBullet)
			iter->Render();
	}



	PlayerBullet * PlayerAttackManager::CreatePlayerBullet(const BulletType & type, const D3DXVECTOR3& pos, const D3DXVECTOR3& direction)
	{
		switch (type)
		{
		case BulletType_Default:
			break;
		}
		PlayerBullet* temp = new PlayerBullet(this, m_playerBulletModel[L"TEST"]);
		temp->SetPosition(pos);
		temp->SetDirection(direction);
		m_playerBullet.push_back(temp);
		return temp;
	}

	MapTile* PlayerAttackManager::CheckCrashMapTile(const D3DXVECTOR3& pos, const int & tile)
	{
		return (*m_mapManager)->CheckCrashWithMap(tile, pos, 3);
	}

	bool PlayerAttackManager::IsCrashedByWall(const D3DXVECTOR3 & pos)
	{
		return (*m_mapManager)->OutOfMap(pos, 3);
	}

	Enemy * PlayerAttackManager::CheckCrashEnemy(const D3DXVECTOR3 & pos)
	{
		return (*m_enemyManager)->CrashWithBullet(pos, 3);
	}

}
