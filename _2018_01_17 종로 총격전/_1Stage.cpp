#include "stdafx.h"
#include "_1Stage.h"

#include "Player.h"
#include "Enemy.h"
#include "Attack.h"
#include "SlowObject.h"
#include "BlockObject.h"
#include "House.h"
#include "Insergent.h"
#include "Grenade.h"
#include "Item.h"

#include "Effect.h"
#include "EnemyDead.h"
#include "Maybe_Gun_Effect.h"
#include "GrenadeEffect.h" 

#include "Fade.h"
#include "Warning.h"

_1Stage::_1Stage()
	: m_player(nullptr)
	, m_insergent(nullptr)

	, m_stageProgress(0)
	, m_enemyRush(0)
	, m_playerAutoMoving(0)

	, m_fade(nullptr)
	, m_warning(nullptr)

	, m_insergentCountwatisfackingthesdasdsadaada(0)
{
}
_1Stage::~_1Stage()
{
}



void _1Stage::Initialize()
{

	m_player = (nullptr);
	m_insergent = (nullptr);
	
	m_stageProgress = (0);
	m_enemyRush = (0);
	m_playerAutoMoving = (0);
	
	m_fade = (nullptr);
	m_warning = (nullptr);
	
	m_insergentCountwatisfackingthesdasdsadaada = (0);


	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"BG0", L"./Resource/map/background0.png");
	///GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"BG1", a"./Resource/map/background1.png");
	///GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"BG2", L"./Resource/map/background2.png");
	///GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"BG3", L"./Resource/map/background3.png");
	///GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"BG4", L"./Resource/map/background4.png");
	///GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"BG5", L"./Resource/map/background5.png");
	///GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Cloud0", L"./Resource/map/cloud0.png");
	///GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Cloud1", L"./Resource/map/cloud1.png");
	///GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Cloud2", L"./Resource/map/cloud2.png");

	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Player0", L"./Resource/player/player0.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Player1", L"./Resource/player/player1.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Aim", L"./Resource/player/aim.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Gun", L"./Resource/player/gun.png");

	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Enemy0", L"./Resource/enemy/enemy0.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Enemy1", L"./Resource/enemy/enemy1.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"EnemyGun", L"./Resource/enemy/gun.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"EnemyDead0", L"./Resource/enemy/dead0.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"EnemyDead1", L"./Resource/enemy/dead1.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"EnemyDead2", L"./Resource/enemy/dead2.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"EnemyDead3", L"./Resource/enemy/dead3.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Insergent0", L"./Resource/enemy/insergent0.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Insergent1", L"./Resource/enemy/insergent1.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Insergent2", L"./Resource/enemy/insergent2.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Insergent3", L"./Resource/enemy/insergent3.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Attack", L"./Resource/player/attack.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"EnemyAttack", L"./Resource/enemy/attack.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"InsergentAttack", L"./Resource/enemy/insergentAttack.png");

	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"MoveEffect0", L"./Resource/player/moveEffect0.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"MoveEffect1", L"./Resource/player/moveEffect1.png");

	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Ground", L"./Resource/map/ground.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"House0", L"./Resource/map/house0.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"House1", L"./Resource/map/house1.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"House2", L"./Resource/map/house2.png");

	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Object_Tile", L"./Resource/object/tile.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Object_Cat", L"./Resource/object/cat.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Object_Box", L"./Resource/object/box.png");

	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"F0", L"./Resource/effect/dlrp_dho_dlvprxmdi0.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"F1", L"./Resource/effect/dlrp_dho_dlvprxmdi1.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"F2", L"./Resource/effect/dlrp_dho_dlvprxmdi2.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"F3", L"./Resource/effect/dlrp_dho_dlvprxmdi3.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"F4", L"./Resource/effect/dlrp_dho_dlvprxmdi4.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"F5", L"./Resource/effect/dlrp_dho_dlvprxmdi5.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"FB0", L"./Resource/effect/boom_effect_0.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"FB1", L"./Resource/effect/boom_effect_1.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"FB2", L"./Resource/effect/boom_effect_2.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"FB3", L"./Resource/effect/boom_effect_3.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"FB4", L"./Resource/effect/boom_effect_4.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"FB5", L"./Resource/effect/boom_effect_5.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"FB6", L"./Resource/effect/boom_effect_6.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"FB7", L"./Resource/effect/boom_effect_7.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"FB8", L"./Resource/effect/boom_effect_8.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"FB9", L"./Resource/effect/boom_effect_9.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"FB10", L"./Resource/effect/boom_effect_10.png");

	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Grenade", L"./Resource/player/grenade.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"G0", L"./Resource/effect/grenade_effect_0.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"G1", L"./Resource/effect/grenade_effect_1.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"G2", L"./Resource/effect/grenade_effect_2.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"G3", L"./Resource/effect/grenade_effect_3.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"G4", L"./Resource/effect/grenade_effect_4.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"G5", L"./Resource/effect/grenade_effect_5.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"G6", L"./Resource/effect/grenade_effect_6.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"G7", L"./Resource/effect/grenade_effect_7.png");

	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Num0", L"./Resource/char/0.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Num1", L"./Resource/char/1.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Num2", L"./Resource/char/2.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Num3", L"./Resource/char/3.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Num4", L"./Resource/char/4.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Num5", L"./Resource/char/5.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Num6", L"./Resource/char/6.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Num7", L"./Resource/char/7.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Num8", L"./Resource/char/8.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Num9", L"./Resource/char/9.png");

	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"UI_Player", L"./Resource/ui/p_frame.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"UI_Player_Health", L"./Resource/ui/p_health.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"UI_Player_Skill", L"./Resource/ui/p_skill.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"UI_Player_Grenade", L"./Resource/ui/p_grenade.png");

	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Item_Burst",		L"./Resource/item/burst.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Item_PowerUp",	L"./Resource/item/power.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Item_Grenade",	L"./Resource/item/grenade.png");

	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"UI_Boss0_Frame", L"./Resource/ui/frame0.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"UI_Boss0_Health", L"./Resource/ui/health0.png");


	m_player = new Player(m_camera, &m_playerAttack, &m_grenade);
	m_player->Initialize();

	Enemy* enemy = new Enemy();
	enemy->Initialize(m_player, &m_enemyAttack);
	enemy->SetPosition(D3DXVECTOR2(rand() % 2000 - 1000, rand() % 400));
	m_enemy.push_back(enemy);


	bool slowCreated = false;
	for (size_t i = 0; i < 32; i++)
	{
		House* house = new House();
		house->Initialize(D3DXVECTOR2(((int)i - 2) * 300 + (i / 2) * 100, 0), rand() % 3);
		m_house.push_back(house);

		if (!slowCreated && (i % 2 || rand() % 2))
		{
			SlowObject* slowObject = new SlowObject();
			slowObject->Initialize(D3DXVECTOR2(((int)i - 2) * 300 + (i / 2) * 100 + rand() % 200 - 100, 136 + 21));
			m_slowObject.push_back(slowObject);
			slowCreated = true;
		}

		if (i % 2)
			slowCreated = false;
	}

	auto CreateBlock = [](std::vector<BlockObject*>* container, const D3DXVECTOR2& position)
	{
		BlockObject* blockObject = new BlockObject();
		blockObject->Initialize(position, D3DXVECTOR2(13.5, 13.5), GetSingleton(TextureLoader)->GetTexture(L"Object_Box"));
		container->push_back(blockObject);
	};
	for (size_t i = 0; i < 10000;)
	{
		if (rand() % 2)
		{
			CreateBlock(&m_blockObject, D3DXVECTOR2(i, -155));
			CreateBlock(&m_blockObject, D3DXVECTOR2(i + 27, -155));
			CreateBlock(&m_blockObject, D3DXVECTOR2(i + 27, -155 + 27));
			if (rand() % 2)
				CreateBlock(&m_blockObject, D3DXVECTOR2(i, -155 + 27));
			if (rand() % 2)
			{
				CreateBlock(&m_blockObject, D3DXVECTOR2(i + 54, -155));
				CreateBlock(&m_blockObject, D3DXVECTOR2(i + 54, -155 + 27));
				CreateBlock(&m_blockObject, D3DXVECTOR2(i + 27 + 54, -155));
				CreateBlock(&m_blockObject, D3DXVECTOR2(i + 27 + 54, -155 + 27));
				if (rand() % 2)
				{
					CreateBlock(&m_blockObject, D3DXVECTOR2(i + 54, -155 + 54));
					CreateBlock(&m_blockObject, D3DXVECTOR2(i + 27 + 54, -155 + 54));
					CreateBlock(&m_blockObject, D3DXVECTOR2(i + 27 + 54, -155 + 27 + 54));
					CreateBlock(&m_blockObject, D3DXVECTOR2(i + 54, -155 + 27 + 54));
				}
			}
		}

		i += 200 + rand() % 300;
	}

	m_stageProgress = 0;
	m_enemyRush = 0;
	m_playerAutoMoving = 90;

	m_fade = new Fade();
	m_fade->Initialize(true);
}

void _1Stage::Update()
{

	float playerPosition = m_player->GetPosition().x;
	auto SummonEnemy = [](Player* player, std::vector<Enemy*>* enemyContainer, std::vector<Attack*>* enemyAttackContainer, const D3DXVECTOR2& position)
	{
		Enemy* enemy = new Enemy();
		enemy->Initialize(player, enemyAttackContainer);
		enemy->SetPosition(position);
		enemyContainer->push_back(enemy);
	};
	float cameraPosition = m_camera.GetPosition().x;
	float cameraSize = m_camera.GetSize().x;
	{
		switch (m_stageProgress)
		{
		case 0:
			if (playerPosition > 600 * 2)
			{
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition + cameraSize + rand() % 200, -150));
				m_stageProgress++;
			}
		case 1:
			if (playerPosition > 900 * 2)
			{
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition + cameraSize + rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition + cameraSize + rand() % 200, -150));
				m_stageProgress++;
			}
		case 2:
			if (playerPosition > 1200 * 2)
			{
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition - cameraSize - rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition + cameraSize + rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition + cameraSize + rand() % 200, -150));
				m_stageProgress++;
			}
		case 3:
			if (playerPosition > 1500 * 2)
			{
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition - cameraSize - rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition - cameraSize - rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition + cameraSize + rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition + cameraSize + rand() % 200, -150));
				m_stageProgress++;
			}
		case 4:
			if (playerPosition > 1800 * 2)
			{
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition - cameraSize - rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition - cameraSize - rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition + cameraSize + rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition + cameraSize + rand() % 200, -150));
				m_stageProgress++;
			}
		case 5:
			if (playerPosition > 2100 * 2)
			{
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition - cameraSize - rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition - cameraSize - rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition - cameraSize - rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition + cameraSize + rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition + cameraSize + rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition + cameraSize + rand() % 200, -150));
				m_stageProgress++;
			}
		case 6:
			if (playerPosition > 2400 * 2)
			{
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition - cameraSize - rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition - cameraSize - rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition - cameraSize - rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition + cameraSize + rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition + cameraSize + rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition + cameraSize + rand() % 200, -150));
				m_stageProgress++;
			}
		case 7:
			if (playerPosition > 2700 * 2)
			{
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition - cameraSize - rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition - cameraSize - rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition - cameraSize - rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition - cameraSize - rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition + cameraSize + rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition + cameraSize + rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition + cameraSize + rand() % 200, -150));
				SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition + cameraSize + rand() % 200, -150));
				m_stageProgress++;
			}
		case 8:
			if (playerPosition > 3000 * 2)
			{
				if (m_warning)
				{
					m_warning->Update();
					if (m_warning->PleaseDeleteThis())
					{
						SAFE_RELEASE_DELETE(m_warning);

						m_insergent = new Insergent();
						m_insergent->Initialize(&m_enemyAttack);
					}
				}
				else if (!m_insergent)
				{
					m_warning = new Warning();
					m_warning->Initialize();
				}

				if (m_enemyRush > 180)
				{
					m_enemyRush = 0;
					if (m_enemy.size() <= 10)
					{
						SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition - cameraSize - rand() % 200, -150));
						SummonEnemy(m_player, &m_enemy, &m_enemyAttack, D3DXVECTOR2(cameraPosition + cameraSize + rand() % 200, -150));
					}
				}
				m_enemyRush++;
			}
		}
	}



	if (m_fade && m_fade->IsEnd())
	{
		if (!m_fade->IsFadeIn())
			GetSingleton(SceneManager)->ChangeScene(L"Story");
		SAFE_RELEASE_DELETE(m_fade);
		return;
	}
	if (!m_player->GetHealth())
	{
		GetSingleton(SceneManager)->ChangeScene(L"Stage1");
		return;
	}

	//House
	for (auto& iter : m_house)
	{
		iter->Update();

		///	//Infinity
		///	if (iter->GetPosition().x + 150 < m_camera.GetPosition().x - m_camera.GetSize().x)
		///		iter->Initialize(GetSingleton(TextureLoader)->GetTexture(std::wstring(L"House") + (rand() % 2 ? L"1" : L"0")), iter->GetPosition() + D3DXVECTOR2(1200, 0));
		///	else if (iter->GetPosition().x - 150 > m_camera.GetPosition().x + m_camera.GetSize().x)
		///		iter->Initialize(GetSingleton(TextureLoader)->GetTexture(std::wstring(L"House") + (rand() % 2 ? L"1" : L"0")), iter->GetPosition() - D3DXVECTOR2(1200, 0));

		//Another Object Position Calculate
		auto& lineList = iter->GetLine().GetVertex();
		for (auto& iter : lineList)
		{
			const D3DXVECTOR2& v1 = iter.first;
			const D3DXVECTOR2& v2 = iter.second;

			float result;
			auto YPositionDeduction = [&v1, &v2](float& out, const D3DXVECTOR2& pivot, const float& gravity)->bool
			{
				if (gravity < 0)
					return false;

				if (v1.x <= pivot.x && pivot.x <= v2.x)
				{
					float lerpScale = (pivot.x - v1.x) / (v2.x - v1.x);
					float lerpedY = v1.y + (v2.y - v1.y) * lerpScale;
					if (pivot.y <= lerpedY && pivot.y + gravity + 10 >= lerpedY)
					{
						out = lerpedY;
						return true;
					}
				}
				return false;
			};
			
			D3DXVECTOR2 playerPosition = m_player->GetPosition();
			if (YPositionDeduction(result, playerPosition - D3DXVECTOR2(0, m_player->GetAABB().GetSize().y), m_player->GetGravity()))
			{
				m_player->SetPosition(D3DXVECTOR2(playerPosition.x, result + m_player->GetAABB().GetSize().y));
				m_player->SetGravity(0);
				m_player->SetGround();
			}
			for (auto& iter : m_playerAttack)
			{
				if (rand() % 4 == 0 && YPositionDeduction(result, iter->GetPosition(), 1))
					iter->Expire();
				if (iter->GetPosition().y < -160)
					iter->Expire();
			}
			for (auto& iter : m_enemyAttack)
			{
				if (rand() % (iter->IsFake() ? 3 : 5) == 0 && YPositionDeduction(result, iter->GetPosition(), 1))
					iter->Expire();
				if (iter->GetPosition().y < -160)
					iter->Expire();
			}
		}
	}

	//Player
	 m_player->AutoMovingActivate(m_playerAutoMoving);
	 if (m_playerAutoMoving) m_playerAutoMoving--;
	 m_player->Update();
	 if (m_player->GetPosition().x < -m_camera.GetSize().x + 200)
	 	m_player->SetPosition(D3DXVECTOR2(-m_camera.GetSize().x + 200, m_player->GetPosition().y));
	 
	D3DXVECTOR2 ppos = m_player->GetPosition();
	D3DXVECTOR2 psize = m_player->GetAABB().GetSize();
	for (auto& iter : m_blockObject)
	{
		D3DXVECTOR2 ipos = iter->GetPosition();
		D3DXVECTOR2 isize = iter->GetAABB().GetSize();
		if (m_player->GetAABB().CalculateCollision(iter->GetAABB()))
		{
			if (ppos.y - psize.y + m_player->GetGravity() + 2 >= ipos.y + isize.y)
			{
				ppos.y = ipos.y + isize.y + psize.y;
				m_player->SetPosition(ppos);
				m_player->SetGravity(0);
				m_player->SetGround();
			}
			else
			{
				ppos.x = ppos.x < ipos.x ?
					ipos.x - isize.x - psize.x :
					ipos.x + isize.x + psize.x;
				m_player->SetPosition(ppos);
			}
		}
	}

	if (m_player->GetPosition().y <= -150)
	{
		m_player->SetPosition(D3DXVECTOR2(m_player->GetPosition().x, -150));
		m_player->SetGravity(0);
		m_player->SetGround();
	}
	if (!m_fade && m_player->GetPosition().x > 10000)
	{
		m_fade = new Fade();
		m_fade->Initialize(false);
	}
	for (auto iter = m_item.begin(); iter != m_item.end();)
	{
		if (m_player->GetAABB().CalculateCollision((*iter)->GetAABB()))
		{
			if ((*iter)->GetType() == 2)
				m_player->InsertGrenade();
			else
				m_player->SetSpecialAttack((*iter)->GetType());
			SAFE_RELEASE_DELETE(*iter);
			iter = m_item.erase(iter);
		}
		else
			iter++;
	}

	//Insergent
	if (m_insergent)
	{
		m_insergentCountwatisfackingthesdasdsadaada++;

		m_insergent->Update(D3DXVECTOR2(m_camera.GetPosition()), D3DXVECTOR2(m_camera.GetSize()));
		if (m_insergent->GetHealth() && m_insergent->GetAABB().CalculateCollision(m_player->GetAABB()) && !m_fade)
				m_player->Kill();

		if (!m_insergent->GetHealth() && !(m_insergentCountwatisfackingthesdasdsadaada % 10))
		{
			GrenadeEffect* effect = new GrenadeEffect(m_insergent->GetPosition() + D3DXVECTOR2(rand() % 160 - 80, rand() % 160 - 80));
			m_effect.push_back(effect);
		}
	}


	//SlowObj
	for (auto iter = m_slowObject.begin(); iter != m_slowObject.end();)
	{
		if (m_player->GetAABB().CalculateCollision((*iter)->GetAABB()))
		{
			m_player->SetSlow();
			SAFE_RELEASE_DELETE(*iter);
			iter = m_slowObject.erase(iter);
			continue;
		}

		for (auto iterB = m_playerAttack.begin(); iterB != m_playerAttack.end();)
		{
			if ((*iterB)->GetAABB().CalculateCollision((*iter)->GetAABB()))
			{
				(*iterB)->Expire();
				(*iter)->Hit();
				if (!(*iter)->GetHealth())
				{
					SAFE_RELEASE_DELETE(*iter);
					iter = m_slowObject.erase(iter);
					goto seapeal;
				}

				g_score += 10;
			}
			iterB++;
		}

		for (auto iterB = m_enemyAttack.begin(); *iter && iterB != m_enemyAttack.end();)
		{
			if ((*iterB)->GetAABB().CalculateCollision((*iter)->GetAABB()))
			{
				(*iterB)->Expire();
				(*iter)->Hit();
				if (!(*iter)->GetHealth())
				{
					SAFE_RELEASE_DELETE(*iter);
					iter = m_slowObject.erase(iter);
					goto seapeal;
				}
			}
			iterB++;
		}

		if (*iter)
			iter++;
	}
	seapeal:

	//Enemy
	for (auto iter = m_enemy.begin(); iter != m_enemy.end();)
	{
		(*iter)->Update();
		if ((*iter)->GetPosition().y <= -150)
		{
			(*iter)->SetPosition(D3DXVECTOR2((*iter)->GetPosition().x, -150));
			(*iter)->SetGravity(0);
		}

		if ((*iter)->GetHealth())
			iter++;
		else
		{
			if (rand() % 5)
			{
				Item* item = new Item();
				item->Initialize(rand() % 3, (*iter)->GetPosition());
				m_item.push_back(item);
			}
 
			EnemyDead* effect = new EnemyDead((*iter)->GetPosition(), (*iter)->IsLookAtRight());
			m_effect.push_back(effect);
			SAFE_RELEASE_DELETE(*iter);
			iter = m_enemy.erase(iter);

			g_score += 100;
		}
	}

	//Player Attack
	for (auto iter = m_playerAttack.begin(); iter != m_playerAttack.end();)
	{
		(*iter)->Update();
		for (auto& iterE : m_enemy)
		{
			if ((*iter)->GetAABB().CalculateCollision(iterE->GetAABB()))
			{
				(*iter)->Expire();
				iterE->Hit();
			}
		}
		for (auto& iterB : m_blockObject)
		{
			if ((*iter)->GetAABB().CalculateCollision(iterB->GetAABB()))
				(*iter)->Expire();
		}

		if ((*iter)->IsExpired())
		{
			Maybe_Gun_Effect* effect = new Maybe_Gun_Effect((*iter)->GetPosition());
			m_effect.push_back(effect);
			SAFE_RELEASE_DELETE(*iter);
			iter = m_playerAttack.erase(iter);
		}
		else
			iter++;
	}

	for (auto iter = m_grenade.begin(); iter != m_grenade.end();)
	{
		(*iter)->Update();

		if (m_insergent && m_insergent->GetAABB().CalculateCollision((*iter)->GetAABB()))
		{
			(*iter)->Expire();
			m_insergent->Hit();
		}

		for (auto& iterE : m_enemy)
		{
			if ((*iter)->GetAABB().CalculateCollision(iterE->GetAABB()))
			{
				(*iter)->Expire();
				iterE->Kill();
			}
		}
		for (auto& iterB : m_blockObject)
		{
			if ((*iter)->GetAABB().CalculateCollision(iterB->GetAABB()))
				(*iter)->Expire();
		}

		if ((*iter)->GetPosition().y < -150)
			(*iter)->Expire();

		if ((*iter)->IsExpired())
		{
			GrenadeEffect* effect = new GrenadeEffect((*iter)->GetPosition());
			m_effect.push_back(effect);
			SAFE_RELEASE_DELETE(*iter);
			iter = m_grenade.erase(iter);
		}
		else
			iter++;
	}

	//Enemy Attack
	for (auto iter = m_enemyAttack.begin(); iter != m_enemyAttack.end();)
	{
		(*iter)->Update();

		if (!(*iter)->IsFake())
		{
			if ((*iter)->GetAABB().CalculateCollision(m_player->GetAABB()))
			{
				(*iter)->Expire();
				if (!m_fade)
					m_player->Hit();
			}
		}
		for (auto& iterB : m_blockObject)
		{
			if ((*iter)->GetAABB().CalculateCollision(iterB->GetAABB()))
				(*iter)->Expire();
		}

		if ((*iter)->IsExpired())
		{
			Maybe_Gun_Effect* effect = new Maybe_Gun_Effect((*iter)->GetPosition());
			m_effect.push_back(effect);
			SAFE_RELEASE_DELETE(*iter);
			iter = m_enemyAttack.erase(iter);
		}
		else
			iter++;
	}

	//Effect
	for (auto iter = m_effect.begin(); iter != m_effect.end();)
	{
		(*iter)->Update();

		if ((*iter)->IsExpired())
		{
			SAFE_RELEASE_DELETE(*iter);
			iter = m_effect.erase(iter);
		}
		else
			iter++;
	}


	//Camera
	D3DXVECTOR2 cursor;	POINT _temp = GetSingleton(InputManager)->GetMousePos();	cursor.x = _temp.x;	cursor.y = _temp.y;
	cursor.x = m_player->GetPosition().x + ((cursor.x / 800) - 1) * m_camera.GetSize().x;
	cursor.y = m_player->GetPosition().y + ((cursor.y / 450) - 1) * -m_camera.GetSize().y;

	auto GetVec1Lerp = [](const float& v1, const float& v2, const float& v)->float { return (v - v1) / (v2 - v1); };
	D3DXVECTOR2 cameraPos;
	D3DXVECTOR2 objectivePosition;
	D3DXVec2Lerp(&objectivePosition, &m_player->GetPosition(), &cursor, 0.5f);
	D3DXVec2Lerp(&cameraPos, &D3DXVECTOR2(m_camera.GetPosition()), &objectivePosition, 0.05f);

	if (cameraPos.x < 0) cameraPos.x = 0;
	if (cameraPos.y - m_camera.GetSize().y < -241) cameraPos.y = -241 + m_camera.GetSize().y;
	
	m_camera.SetCameraTranslation(&D3DXVECTOR3(cameraPos.x, cameraPos.y, 0), &D3DXVECTOR3(800 * 0.5f, 450 * 0.5f, 100));


	if (m_fade)
		m_fade->Update();
}

void _1Stage::Render()
{
	m_camera.InputCameraDevice(DEVICE);


	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"BG0"), m_camera.GetPosition() + D3DXVECTOR3(-m_camera.GetSize().x , 0, 100)	, false, D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(0.6f, 0.6f));
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"BG0"), m_camera.GetPosition() + D3DXVECTOR3(0, 0,						 100)	, false, D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(0.6f, 0.6f));
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"BG0"), m_camera.GetPosition() + D3DXVECTOR3(+m_camera.GetSize().x , 0, 100)	, false, D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(0.6f, 0.6f));
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"Ground"), D3DXVECTOR3(((int)m_camera.GetPosition().x / 1025) * 1025, -136.5f - 53, 0));
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"Ground"), D3DXVECTOR3(((int)m_camera.GetPosition().x / 1025 + 1) * 1025, -136.5f -53, 0));


	for (auto& iter : m_house)
		iter->Render();
	for (auto& iter : m_slowObject)
		iter->Render();
	for (auto& iter : m_blockObject)
		iter->Render();

	for (auto& iter : m_enemyAttack)
		iter->Render();
	for (auto& iter : m_playerAttack)
		iter->Render();
	for (auto& iter : m_grenade)
		iter->Render();



	for (auto& iter : m_enemy)
		iter->Render();
	for (auto& iter : m_item)
		iter->Render();

	if (m_insergent)
		m_insergent->Render();

	for (auto& iter : m_effect)
		iter->Render();

	m_player->Render();
	
	if (m_fade)
		m_fade->Render(m_camera.GetPosition(), m_camera.GetSize());

	if (m_warning)
		m_warning->Render(m_camera.GetPosition());

	GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(L"UI_Player_Health"), D3DXVECTOR3(166.5 + 135, 14.5 + 30, 0), D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(1, 1), (float)m_player->GetHealth() / 30.);
	GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(L"UI_Player_Skill"), D3DXVECTOR3(166.5 + 135, 10 + 58, 0), D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(1, 1), (float)m_player->GetSpecialAttackTime() / 600.);
	GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(L"UI_Player_Grenade"), D3DXVECTOR3(46.5 + 149, 35 + 90, 0));
	GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(std::wstring(L"Num") + std::to_wstring(m_player->GetGrenadeCount())), D3DXVECTOR3(46.5 + 149 + 20, 35 + 90 + 10, 0));
	GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(L"UI_Player"), D3DXVECTOR3(225 + 20, 50 + 20, 0));

	if (m_insergent)
	{
		GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(L"UI_Boss0_Health"), D3DXVECTOR3(1600 - 288 + 172 - 85 - 20, 23 + 9 + 20, 0), D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(1, 1), (float)m_insergent->GetHealth() / 3.);
		GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(L"UI_Boss0_Frame"), D3DXVECTOR3(1600 - 144 - 20, 50 + 20, 0));
	}
}

void _1Stage::Release()
{
	SAFE_RELEASE_DELETE(m_player);
	SAFE_RELEASE_DELETE(m_insergent);
	for (auto& iter : m_enemy)
		SAFE_RELEASE_DELETE(iter);
	m_enemy.clear();
	for (auto& iter : m_house)
		SAFE_RELEASE_DELETE(iter);
	m_house.clear();
	for (auto& iter : m_slowObject)
		SAFE_RELEASE_DELETE(iter);
	m_slowObject.clear();
	for (auto& iter : m_blockObject)
		SAFE_RELEASE_DELETE(iter);
	m_blockObject.clear();

	for (auto& iter : m_effect)
		SAFE_RELEASE_DELETE(iter);
	m_effect.clear();

	for (auto& iter : m_playerAttack)
		SAFE_RELEASE_DELETE(iter);
	m_playerAttack.clear();
	for (auto& iter : m_grenade)
		SAFE_RELEASE_DELETE(iter);
	m_grenade.clear();
	for (auto& iter : m_enemyAttack)
		SAFE_RELEASE_DELETE(iter);
	m_enemyAttack.clear();

	SAFE_RELEASE_DELETE(m_fade);


	for (auto& iter : m_enemy)			SAFE_RELEASE_DELETE(iter);
	for (auto& iter : m_grenade)		SAFE_RELEASE_DELETE(iter);
	for (auto& iter : m_playerAttack)	SAFE_RELEASE_DELETE(iter);
	for (auto& iter : m_enemyAttack)	SAFE_RELEASE_DELETE(iter);
	for (auto& iter : m_slowObject)		SAFE_RELEASE_DELETE(iter);
	for (auto& iter : m_blockObject)	SAFE_RELEASE_DELETE(iter);
	for (auto& iter : m_house)			SAFE_RELEASE_DELETE(iter);
	for (auto& iter : m_item)			SAFE_RELEASE_DELETE(iter);
	for (auto& iter : m_effect)			SAFE_RELEASE_DELETE(iter);
	m_enemy.clear();
	m_grenade.clear();
	m_playerAttack.clear();
	m_enemyAttack.clear();
	m_slowObject.clear();
	m_blockObject.clear();
	m_house.clear();
	m_item.clear();
	m_effect.clear();


	SAFE_RELEASE_DELETE(m_player);
	SAFE_RELEASE_DELETE(m_insergent);
	SAFE_RELEASE_DELETE(m_fade);
	SAFE_RELEASE_DELETE(m_warning);
}
