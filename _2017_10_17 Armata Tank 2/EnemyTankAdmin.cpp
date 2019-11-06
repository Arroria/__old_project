#include "stdafx.h"
#include "EnemyTankAdmin.h"

#include "PlayerAdmin.h"
#include "ShellAdmin.h"
#include "Map.h"
#include "Item.h"

#include "EnemyTank.h"
#include "BaseBlock.h"

EnemyTankAdmin::EnemyTankAdmin(PlayerAdmin** playerAdmin, ShellAdmin** shellAdmin, Map** map, std::list<Item*>* itemContainer)
	: m_playerAdmin(playerAdmin)
	, m_shellAdmin(shellAdmin)
	, m_map(map)
	, m_itemContainer(itemContainer)
{
}
EnemyTankAdmin::~EnemyTankAdmin()
{
	for (auto& iter : m_enemyTank)
		SAFE_DELETE(iter);
	m_enemyTank.clear();
}


bool EnemyTankAdmin::RegisterEnemyTank(BaseTank * enemy)
{
	if (!enemy)
		return true;

	m_enemyTank.push_back(enemy);
	((EnemyTank*)enemy)->SetEnemyTankAdmin(this);
	return false;
}

bool EnemyTankAdmin::EnemyUpdate()
{
	auto enemyIter = m_enemyTank.begin();
	while (enemyIter != m_enemyTank.end())
	{
		(*enemyIter)->Update();

		//Position Correction func
		//Func will optimum to turn
		auto PositionCorrection = [&](BaseTank* player, const D3DXVECTOR3& targetPosition, const D3DXVECTOR2& targetCollisionSize)
		{
			D3DXVECTOR3& playerPosition(player->GetPosition());
			D3DXVECTOR2& playerCollisionSize(player->GetCollisionSize());

			D3DXVECTOR2 difference;
			difference.x = fabs(playerPosition.x - targetPosition.x);
			difference.y = fabs(playerPosition.y - targetPosition.y);

			D3DXVECTOR2 crashedRange;
			crashedRange.x = (playerCollisionSize.x + targetCollisionSize.x) - difference.x;
			crashedRange.y = (playerCollisionSize.y + targetCollisionSize.y) - difference.y;

			if (crashedRange.x > 0 && crashedRange.y > 0)
			{
				(crashedRange.x - crashedRange.y < 0) ?
					(playerPosition.x = (playerPosition.x > targetPosition.x ?
						targetPosition.x + targetCollisionSize.x + playerCollisionSize.x :
						targetPosition.x - targetCollisionSize.x - playerCollisionSize.x)) :
						(playerPosition.y = (playerPosition.y > targetPosition.y ?
							targetPosition.y + targetCollisionSize.y + playerCollisionSize.y :
							targetPosition.y - targetCollisionSize.y - playerCollisionSize.y));
				player->SetPosition(playerPosition);
				((EnemyTank*)(*enemyIter))->SetTurn();
			}
		};

		//Correction for player
		if ((*m_playerAdmin)->GetPlayer())
			PositionCorrection(*enemyIter, (*m_playerAdmin)->GetPlayer()->GetPosition(), (*m_playerAdmin)->GetPlayer()->GetCollisionSize());
		//Correction for another enemy
		for (auto& anotherEnemyIter : m_enemyTank)
		{
			if (*enemyIter == anotherEnemyIter)
				continue;

			PositionCorrection(*enemyIter, anotherEnemyIter->GetPosition(), anotherEnemyIter->GetCollisionSize());
		}
		//Correction for blokc of map
		for (auto& blockIter : *(*m_map)->GetMapBlockContainer())
		{
			if (blockIter->IsCanPenetrateForMove())
				continue;

			PositionCorrection(*enemyIter, blockIter->GetPosition(), blockIter->GetCollisionSize());
		}
		
		if ((*enemyIter)->CheckDestroyThis())
		{
			m_itemContainer->push_back(new Item(rand() % 4, (*enemyIter)->GetPosition()));

			SAFE_DELETE(*enemyIter);
			enemyIter = m_enemyTank.erase(enemyIter);
		}
		else
			enemyIter++;
	}
	return false;
}

bool EnemyTankAdmin::EnemyRender()
{
	for (auto& iter : m_enemyTank)
		iter->Render();
	return false;
}

bool EnemyTankAdmin::CreateShell(BaseShell * shell)
{
	if (!shell || !m_shellAdmin)
		return true;

	(*m_shellAdmin)->RegisterEnemyShell(shell);
	return false;
}
