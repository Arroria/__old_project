#include "stdafx.h"
#include "PlayerAdmin.h"

#include "ShellAdmin.h"
#include "EnemyTankAdmin.h"
#include "Map.h"
#include "Item.h"

#include "BaseBlock.h"

#include "PlayerTank.h"

PlayerAdmin::PlayerAdmin(ShellAdmin** shellAdmin, EnemyTankAdmin** enemyAdmin, Map** map, std::list<Item*>* itemContainer)
	: m_player(nullptr)
	, m_shellAdmin(shellAdmin)
	, m_enemyAdmin(enemyAdmin)
	, m_map(map)
	, m_itemContainer(itemContainer)
{
}
PlayerAdmin::~PlayerAdmin()
{
	SAFE_DELETE(m_player);
}


BaseTank* PlayerAdmin::GetPlayer() { return m_player; }


bool PlayerAdmin::CreateShell(BaseShell * shell)
{
	if (!shell || !m_shellAdmin)
		return true;

	(*m_shellAdmin)->RegisterPlayerShell(shell);
	return false;
}

bool PlayerAdmin::RegisterPlayer(PlayerTank * player)
{
	if (m_player || !player)
		return true;

	m_player = player;
	m_player->SetPlayerAdmin(this);
	return false;
}

bool PlayerAdmin::PlayerUpdate()
{
	if (GetSingleton(InputManager)->IsKeyDown('1'))	m_player->ChangeWeapon(PlayerTankWeaponType::Cannon);
	if (GetSingleton(InputManager)->IsKeyDown('2'))	m_player->ChangeWeapon(PlayerTankWeaponType::AutoCannon);
	if (GetSingleton(InputManager)->IsKeyDown('3'))	m_player->ChangeWeapon(PlayerTankWeaponType::Rocket);


	if (m_player)
	{
		m_player->Update();

		//Position Correction func
		auto PositionCorrection = [&](BaseTank* player, const D3DXVECTOR3& targetPosition, const D3DXVECTOR2& targetCollisionSize)
		{
			D3DXVECTOR3& playerPosition(player->GetPosition());
			D3DXVECTOR2& playerCollisionSize(player->GetCollisionSize());

			D3DXVECTOR2 difference;
			difference.x = fabs(playerPosition.x - targetPosition.x);
			difference.y = fabs(playerPosition.y - targetPosition.y);

			D3DXVECTOR2 crashedArea;
			crashedArea.x = (playerCollisionSize.x + targetCollisionSize.x) - difference.x;
			crashedArea.y = (playerCollisionSize.y + targetCollisionSize.y) - difference.y;

			if (crashedArea.x > 0 && crashedArea.y > 0)
			{
				(crashedArea.x - crashedArea.y < 0) ?
					(playerPosition.x = (playerPosition.x > targetPosition.x ?
						targetPosition.x + targetCollisionSize.x + playerCollisionSize.x :
						targetPosition.x - targetCollisionSize.x - playerCollisionSize.x )):
					(playerPosition.y = (playerPosition.y > targetPosition.y ?
						targetPosition.y + targetCollisionSize.y + playerCollisionSize.y :
						targetPosition.y - targetCollisionSize.y - playerCollisionSize.y));
				player->SetPosition(playerPosition);
			}
		};

		//Correction for enemy
		for (auto& iter : *(*m_enemyAdmin)->GetEnemyContainer())
			PositionCorrection(m_player, iter->GetPosition(), iter->GetCollisionSize());
		//Correction for block of map
		for (auto& iter : *(*m_map)->GetMapBlockContainer())
		{
			if (iter->IsCanPenetrateForMove())
				continue;
			PositionCorrection(m_player, iter->GetPosition(), iter->GetCollisionSize());
		}
		//Eat item
		for (auto& itemIter : *m_itemContainer)
		{
			D3DXVECTOR2 playerCollider = m_player->GetCollisionSize();
			D3DXVECTOR3 playerPosition = m_player->GetPosition();

			D3DXVECTOR2 itemCollider = itemIter->GetCollisionSize();
			D3DXVECTOR3 itemPosition = itemIter->GetPosition();

			if (playerPosition.x - playerCollider.x < itemPosition.x + itemCollider.x &&
				playerPosition.x + playerCollider.x > itemPosition.x - itemCollider.x &&
				playerPosition.y - playerCollider.y < itemPosition.y + itemCollider.y &&
				playerPosition.y + playerCollider.y > itemPosition.y - itemCollider.y)
				itemIter->EffectForPlayer(m_player);
		}

		if (m_player->CheckDestroyThis())
			SAFE_DELETE(m_player);
		return false;
	}
	return true;
}

bool PlayerAdmin::PlayerRender()
{
	if (m_player)
		m_player->Render();
	return false;
}
