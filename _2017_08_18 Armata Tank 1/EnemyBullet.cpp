#include "MainHeader.h"
#include "EnemyBullet.h"

#include "Map.h"
#include "Player.h"
#include "EnemyBulletManager.h"

namespace AF
{
	EnemyBullet::EnemyBullet(EnemyBulletManager * enemyBulletManager, ASEModel * bulletModel)
		: m_enemyBulletManager(enemyBulletManager)
		, m_bulletModel(bulletModel)
		, m_delete(false)
		, m_crashedTile(nullptr)
	{
	}
	EnemyBullet::~EnemyBullet()
	{
	}



	void EnemyBullet::Initialize()
	{
	}
	
	void EnemyBullet::Update()
	{

		m_position += m_direction * 10;

		MapTile* tile = nullptr;
		Player* player = nullptr;
		for (int temp = 1; temp < 6 && !tile; temp++)
		{
			if (temp == 2) continue;
			tile = m_enemyBulletManager->CheckCrashMapTile(m_position, temp);
		}

		if (tile)
		{
			tile->CrashDamage(5);
			m_delete = true;
		}
		else if (m_enemyBulletManager->IsCrashedByWall(m_position))
			m_delete = true;
		else if (player = m_enemyBulletManager->CheckCrashPlayer(m_position))
		{
			player->GiveDamage(3);
			m_delete = true;
		}
		else
		{
			MapTile* tempTile = m_enemyBulletManager->CheckCrashMapTile(m_position, 7);
			if (tempTile && tempTile != m_crashedTile)
			{
				m_crashedTile = tempTile;
				float temp = m_direction.z;
				m_direction.z = m_direction.x;
				m_direction.x = temp;
				if (!m_crashedTile->IsLookRight())
					m_direction *= -1;
			}
		}
	}
	
	void EnemyBullet::Render()
	{
		D3DXVECTOR3 temp[12];
		temp[0] = D3DXVECTOR3(0, 3, 0);
		temp[1] = D3DXVECTOR3(cosf(D3DXToRadian(330)), sinf(D3DXToRadian(330)), 0) * 3;
		temp[2] = D3DXVECTOR3(cosf(D3DXToRadian(210)), sinf(D3DXToRadian(210)), 0) * 3;
		temp[0 + 3] = temp[0];
		temp[1 + 3] = temp[2];
		temp[2 + 3] = D3DXVECTOR3(0, 0, 10);
		temp[0 + 6] = temp[2];
		temp[1 + 6] = temp[1];
		temp[2 + 6] = temp[2 + 3];
		temp[0 + 9] = temp[1];
		temp[1 + 9] = temp[0];
		temp[2 + 9] = temp[2 + 3];
		for (auto& iter : temp)
			iter.y += 20;

		AF::g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &GetTransformMatrix());
		AF::g_processManager->GetDevice()->SetFVF(D3DFVF_XYZ);
		AF::g_processManager->GetDevice()->SetTexture(0, nullptr);
		AF::g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 4, &temp[0], sizeof(D3DXVECTOR3));
	}
	
	void EnemyBullet::Release()
	{
	}
}
