#include "MainHeader.h"

#include "PlayerManager.h"
#include "PlayerAttackManager.h"
#include "EnemyManager.h"
#include "EnemyBulletManager.h"
#include "MapManager.h"
#include "TextureManager.h"

#include "ASE.h"
#include "Player.h"

namespace AF
{
	PlayerManager::PlayerManager(InputManager ** inputManager, PlayerAttackManager ** playerAttackManager, EnemyManager ** enemyManager, EnemyBulletManager ** enemyBulletManager, MapManager ** mapManager, TextureManager** textureManager, ItemManager** itemManager)
		: m_playerManager(inputManager)
		, m_playerAttackManager(playerAttackManager)
		, m_enemyManager(enemyManager)
		, m_enemyBulletManager(enemyBulletManager)
		, m_mapManager(mapManager)
		, m_textureManager(textureManager)
		, m_itemManager(itemManager)

		, m_player(nullptr)
		, m_playerLife(0)
	{
		ASELoader aseLoader;

		m_playerModel.insert(std::make_pair( L"Chassis",		aseLoader.LoadAseFile(L"./Resource/Tank/Tank_Body.ase") ));
		m_playerModel.insert(std::make_pair( L"Default",		aseLoader.LoadAseFile(L"./Resource/Tank/Tank_Barrel_Default.ase") ));
		m_playerModel.insert(std::make_pair( L"ShotGun",		aseLoader.LoadAseFile(L"./Resource/Tank/Tank_Barrel_ShotGun.ase") ));
		m_playerModel.insert(std::make_pair( L"Minigun_Body",	aseLoader.LoadAseFile(L"./Resource/Tank/Tank_Barrel_Minigun_Body.ase") ));
		m_playerModel.insert(std::make_pair( L"Minigun_Barrel",	aseLoader.LoadAseFile(L"./Resource/Tank/Tank_Barrel_Minigun_Barrel.ase") ));
		m_playerModel.insert(std::make_pair( L"Missile",		aseLoader.LoadAseFile(L"./Resource/Tank/Tank_Barrel_Missile.ase") ));
		m_playerModel.insert(std::make_pair( L"Flamethrower",	aseLoader.LoadAseFile(L"./Resource/Tank/Tank_Barrel_Flamethrower.ase") ));
		m_playerModel.insert(std::make_pair( L"Plasma",			aseLoader.LoadAseFile(L"./Resource/Tank/Tank_Barrel_Plasma.ase") ));


		(*m_textureManager)->LoadTexture("TankLife",		L"./Resource/GameUI/IngameUI/life.png");
		(*m_textureManager)->LoadTexture("TankLifeTe",		L"./Resource/GameUI/IngameUI/life2.png");
		(*m_textureManager)->LoadTexture("TankHealth",		L"./Resource/GameUI/IngameUI/hp2.png");
		(*m_textureManager)->LoadTexture("TankHealthTe",	L"./Resource/GameUI/IngameUI/hp.png");
		(*m_textureManager)->LoadTexture("ItemStar",		L"./Resource/GameUI/IngameUI/item1.png");
		(*m_textureManager)->LoadTexture("ItemBarrior",		L"./Resource/GameUI/IngameUI/item2.png");
		(*m_textureManager)->LoadTexture("ItemHeal",		L"./Resource/GameUI/IngameUI/item3.png");
	}
	PlayerManager::~PlayerManager()
	{
	}



	void PlayerManager::Initialize()
	{
		m_playerLife = 4;
	}

	void PlayerManager::Update()
	{
		if (m_player)
			m_player->Update();
	}

	void PlayerManager::Render()
	{
		if (m_player)
			m_player->Render();
	}

	void PlayerManager::RenderUI()
	{
		//±ÍÂúÀº ÅÊÅ© ³²Àº ¶óÀÌÇÁ ±×¸®±â! ¿Í¾Æ¾Æ¤¿¤¿¾Æ¾Æ¤¿¤¿
		{
			auto lifeTx = (*m_textureManager)->GetTexture("TankLife");
			auto lifeTeTx = (*m_textureManager)->GetTexture("TankLifeTe");
			for (int tempLife = 0; tempLife < 3; tempLife++)
			{
				D3DXMATRIX matS, matT;
				D3DXMatrixScaling(&matS, 0.5f, 0.5f, 0.5f);
				D3DXMatrixTranslation(&matT, 64 + tempLife * 40, 100, 0);
				(*m_textureManager)->SetTransform(matS * matT);
				(*m_textureManager)->TextureRender(m_playerLife > tempLife ? lifeTx : lifeTeTx);
			}
		}

		//±ÍÂúÀº ÅÊÅ© Ã¼·Â ±×¸®±â! ¿Í¾Æ¾Æ¤¿¤¿¾Æ¾Æ¤¿¤¿
		{
			D3DXMATRIX mat;
			D3DXMatrixTranslation(&mat, 64, 180, 0);

			(*m_textureManager)->SetTransform(mat);
			(*m_textureManager)->TextureRender((*m_textureManager)->GetTexture("TankHealthTe"));
			if (m_player)
			{
				auto tempTx = (*m_textureManager)->GetTexture("TankHealth");
				RECT ¾Æ_±ÍÂú¾Æ_¤Ð;
				SetRect(&¾Æ_±ÍÂú¾Æ_¤Ð, 0, 0, m_player->GetHealth() * tempTx->imageInfo.Width / 100, tempTx->imageInfo.Height);

				(*m_textureManager)->SetTransform(mat);
				(*m_textureManager)->TextureRender(tempTx, &¾Æ_±ÍÂú¾Æ_¤Ð);
			}
		}

	}



	void PlayerManager::CreatePlayer(const D3DXVECTOR3& pos)
	{
		if (!m_player)
		{
			Player::Player3DModel	playerModel;
			Player::PlayerInfo		playerInfo;

			playerModel.chassis = m_playerModel[L"Chassis"];
			playerModel.turret = m_playerModel[L"Default"];
			playerModel.gun = nullptr;

			m_player = new AF::Player(this, playerModel, playerInfo);
			m_player->SetPosition(pos);
			m_playerLife--;
		}
	}

	void PlayerManager::DestroyPlayer()
	{
		SAFE_DELETE(m_player);
	}

	bool PlayerManager::CheckCrashPlayerPlease(const D3DXVECTOR3 & pos, const int & size)
	{
		if (!m_player)
			return false;
		D3DXVECTOR3 playerPos = m_player->GetPosition();
		return	pos.x - size < playerPos.x + 22 &&
				pos.x + size > playerPos.x - 22 &&
				pos.z - size < playerPos.z + 22 &&
				pos.z + size > playerPos.z - 22;
	}

#define mapsize 10
	bool PlayerManager::CheckCrashedMap(const D3DXVECTOR3 & nextPos)
	{
		return	(*m_mapManager)->CheckCrashWithMap(1, nextPos, 22) ||
				(*m_mapManager)->CheckCrashWithMap(2, nextPos, 22) ||
				(*m_mapManager)->CheckCrashWithMap(3, nextPos, 22) ||
				(*m_mapManager)->CheckCrashWithMap(4, nextPos, 22) ||
				(*m_mapManager)->CheckCrashWithMap(5, nextPos, 22) ||
				(*m_mapManager)->CheckCrashWithMap(7, nextPos, 22) ||
				(*m_mapManager)->OutOfMap(nextPos, 22);
	}

	/*
	void PlayerManager::Initialize()
	{
		//ASELoader aseLoader;
		//
		//aseLoader.LoadAseFile(L"./Resource/Map/Map_Volcano_World2.ase");
		//
		//m_playerModel.insert(std::make_pair( L"Body",			aseLoader.LoadAseFile(L"./Resource/Tank/Tank_Body.ase") ));
		//m_playerModel.insert(std::make_pair( L"Default",		aseLoader.LoadAseFile(L"./Resource/Tank/Tank_Barrel_Default.ase") ));
		//m_playerModel.insert(std::make_pair( L"ShotGun",		aseLoader.LoadAseFile(L"./Resource/Tank/Tank_Barrel_ShotGun.ase") ));
		//m_playerModel.insert(std::make_pair( L"Minigun_Body",	aseLoader.LoadAseFile(L"./Resource/Tank/Tank_Barrel_Minigun_Body.ase") ));
		//m_playerModel.insert(std::make_pair( L"Minigun_Barrel",	aseLoader.LoadAseFile(L"./Resource/Tank/Tank_Barrel_Minigun_Barrel.ase") ));
		//m_playerModel.insert(std::make_pair( L"Missile",		aseLoader.LoadAseFile(L"./Resource/Tank/Tank_Barrel_Missile.ase") ));
		//m_playerModel.insert(std::make_pair( L"Flamethrower",	aseLoader.LoadAseFile(L"./Resource/Tank/Tank_Barrel_Flamethrower.ase") ));
		//m_playerModel.insert(std::make_pair( L"Plasma",			aseLoader.LoadAseFile(L"./Resource/Tank/Tank_Barrel_Plasma.ase") ));
	}*/
}
