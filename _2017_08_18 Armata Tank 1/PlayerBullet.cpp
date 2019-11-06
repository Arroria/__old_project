#include "MainHeader.h"
#include "PlayerBullet.h"

#include "ASE.h"
#include "PlayerAttackManager.h"
#include "Map.h"
#include "Enemy.h"

namespace AF
{
	PlayerBullet::PlayerBullet(PlayerAttackManager * playerBulletManager, ASEModel * bulletModel)
		: m_playerBulletManager(playerBulletManager)
		, m_bulletModel(bulletModel)
		, m_delete(false)
		, m_crashedTile(nullptr)
	{
	}
	PlayerBullet::~PlayerBullet()
	{
	}



	void PlayerBullet::Initialize()
	{
	}

	void PlayerBullet::Update()
	{
		m_position += m_direction * 10;

		CRASHYEAHHA();
		MapTile* tempTile = m_playerBulletManager->CheckCrashMapTile(m_position, 7);
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

	void PlayerBullet::Render()
	{
		D3DXVECTOR3 temp[12];
		temp[0] = D3DXVECTOR3(0, 3, 0);
		temp[1] = D3DXVECTOR3(cosf(D3DXToRadian( 330 )), sinf(D3DXToRadian( 330 )), 0) * 3;
		temp[2] = D3DXVECTOR3(cosf(D3DXToRadian( 210 )), sinf(D3DXToRadian( 210 )), 0) * 3;
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
	
		//m_bulletModel->SetTransform(GetTransformMatrix());
		//m_bulletModel->Update();
		//m_bulletModel->Render();
	}

	void PlayerBullet::Release()
	{
	}

#define DAMAGE 24


	void PlayerBullet::CRASHYEAHHA()
	{
		MapTile* tile = nullptr;
		for (int temp = 1; temp < 6 && !tile; temp++)
		{
			if (temp == 2) continue;
			tile = m_playerBulletManager->CheckCrashMapTile(m_position, temp);
		}
		if (tile || m_playerBulletManager->IsCrashedByWall(m_position))
		{
			if (tile)
				tile->CrashDamage(DAMAGE);
			m_delete = true;
			return;
		}

		Enemy* enemy = m_playerBulletManager->CheckCrashEnemy(m_position);
		if (enemy)
		{
			enemy->GiveDamage(DAMAGE);
			m_delete = true;
			return;
		}
	}
}			