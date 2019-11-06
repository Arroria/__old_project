#include "stdafx.h"
#include "_2Stage.h"

#include "Player.h"
#include "Enemy.h"
#include "Attack.h"
#include "APC.h"
#include "Grenade.h"
#include "Item.h"

#include "BlockObject.h"
#include "EnemyDead.h"
#include "Maybe_Gun_Effect.h"
#include "GrenadeEffect.h"

#include "Warning.h"

_2Stage::_2Stage()
	: m_player(nullptr)
	, m_apc(nullptr)

	, m_attackBlock(nullptr)

	, m_timer(0)

	, m_warning(nullptr)
{
}
_2Stage::~_2Stage()
{
}



void _2Stage::Initialize()
{
	m_road.Clear();
	m_specialPlace.Clear();

	m_player = (nullptr);
	m_apc = (nullptr);

	m_attackBlock = (nullptr);

	m_timer = (0);

	m_warning = (nullptr);

	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"BG0", L"./Resource/map/background0.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"2Stage", L"./Resource/map/2stage.png");

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
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Attack", L"./Resource/player/attack.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"EnemyAttack", L"./Resource/enemy/attack.png");

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

	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Object_Box", L"./Resource/object/box.png");

	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"APC0", L"./Resource/enemy/APC0.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"APC1", L"./Resource/enemy/APC1.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"APC_Barrel", L"./Resource/enemy/APC_barrel.png");

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

	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Item_Burst", L"./Resource/item/burst.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Item_PowerUp", L"./Resource/item/power.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Item_Grenade", L"./Resource/item/grenade.png");

	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"UI_Boss1_Frame", L"./Resource/ui/frame1.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"UI_Boss1_Health", L"./Resource/ui/health1.png");


	m_player = new Player(m_camera, &m_playerAttack, &m_grenade);
	m_player->Initialize();
	m_player->SetPosition(D3DXVECTOR2(0, 10));

	Enemy* enemy = new Enemy();
	enemy->Initialize(m_player, &m_enemyAttack, 1);
	enemy->SetPosition(D3DXVECTOR2(rand() % 2000 - 1000, rand() % 400));
	m_enemy.push_back(enemy);

	m_road.InsertVertex(D3DXVECTOR2(-700, 0), D3DXVECTOR2(700, 0));
	m_specialPlace.InsertVertex(D3DXVECTOR2(-700 + 624, -12 + 566 - 448), D3DXVECTOR2(-700 + 630, -12 + 566 - 448));
	m_specialPlace.InsertVertex(D3DXVECTOR2(-700 + 785, -12 + 566 - 449), D3DXVECTOR2(-700 + 865, -12 + 566 - 449));
	m_specialPlace.InsertVertex(D3DXVECTOR2(-700 + 562, -12 + 566 - 272), D3DXVECTOR2(-700 + 600, -12 + 566 - 272));

	m_attackBlock = new BlockObject();
	m_attackBlock->Initialize(D3DXVECTOR2(-700 + 615, -12 + 556 - 498), D3DXVECTOR2(15, 50), nullptr);

	auto CreateBlock = [](std::vector<BlockObject*>* container, const D3DXVECTOR2& position, const D3DXVECTOR2& size, Texture* texture)
	{
		BlockObject* blockObject = new BlockObject();
		blockObject->Initialize(position, size, texture);
		container->push_back(blockObject);
	};
	CreateBlock(&m_blockObject, D3DXVECTOR2(-700 + 215, -12 + 566 - 525), D3DXVECTOR2(15, 40), nullptr);
	CreateBlock(&m_blockObject, D3DXVECTOR2(-700 + 506, -12 + 566 - 553), D3DXVECTOR2(28, 13), nullptr);
	CreateBlock(&m_blockObject, D3DXVECTOR2(-700 + 488, -12 + 566 - 360), D3DXVECTOR2(10, 100), nullptr);
	CreateBlock(&m_blockObject, D3DXVECTOR2(+700 - 476, -12 + 566 - 360), D3DXVECTOR2(10, 100), nullptr);
	CreateBlock(&m_blockObject, D3DXVECTOR2(700 - 495, -12 + 566 - 553), D3DXVECTOR2(28, 13), nullptr);
	CreateBlock(&m_blockObject, D3DXVECTOR2(700 - 220, -12 + 566 - 525), D3DXVECTOR2(15, 40), nullptr);

	CreateBlock(&m_blockObject, D3DXVECTOR2(-700 + 180,			-12 + 566 - 540),		D3DXVECTOR2(13, 13), GetSingleton(TextureLoader)->GetTexture(L"Object_Box"));
	CreateBlock(&m_blockObject, D3DXVECTOR2(-700 + 180,			-12 + 566 - 540 + 26),	D3DXVECTOR2(13, 13), GetSingleton(TextureLoader)->GetTexture(L"Object_Box"));
	CreateBlock(&m_blockObject, D3DXVECTOR2(-700 + 180 - 26,	-12 + 566 - 540),		D3DXVECTOR2(13, 13), GetSingleton(TextureLoader)->GetTexture(L"Object_Box"));
	CreateBlock(&m_blockObject, D3DXVECTOR2(-700 + 180,			-12 + 566 - 540 + 52),	D3DXVECTOR2(13, 13), GetSingleton(TextureLoader)->GetTexture(L"Object_Box"));
	CreateBlock(&m_blockObject, D3DXVECTOR2(-700 + 180 - 26,	-12 + 566 - 540 + 26),	D3DXVECTOR2(13, 13), GetSingleton(TextureLoader)->GetTexture(L"Object_Box"));
	
	CreateBlock(&m_blockObject, D3DXVECTOR2(1225 - 700 + 13,		-12 + 566 - 540),		D3DXVECTOR2(13, 13), GetSingleton(TextureLoader)->GetTexture(L"Object_Box"));
	CreateBlock(&m_blockObject, D3DXVECTOR2(1225 - 700 + 13,		-12 + 566 - 540 + 26),	D3DXVECTOR2(13, 13), GetSingleton(TextureLoader)->GetTexture(L"Object_Box"));
	CreateBlock(&m_blockObject, D3DXVECTOR2(1225 - 700 + 13 - 26,	-12 + 566 - 540),		D3DXVECTOR2(13, 13), GetSingleton(TextureLoader)->GetTexture(L"Object_Box"));
	CreateBlock(&m_blockObject, D3DXVECTOR2(1225 - 700 + 13 - 26,	-12 + 566 - 540 + 26),	D3DXVECTOR2(13, 13), GetSingleton(TextureLoader)->GetTexture(L"Object_Box"));
	CreateBlock(&m_blockObject, D3DXVECTOR2(1225 - 700 + 13 - 26,	-12 + 566 - 540 + 52),	D3DXVECTOR2(13, 13), GetSingleton(TextureLoader)->GetTexture(L"Object_Box"));

	CreateBlock(&m_blockObject, D3DXVECTOR2(28, 8),			D3DXVECTOR2(13, 13), GetSingleton(TextureLoader)->GetTexture(L"Object_Box"));
	CreateBlock(&m_blockObject, D3DXVECTOR2(28 + 26, 8),	D3DXVECTOR2(13, 13), GetSingleton(TextureLoader)->GetTexture(L"Object_Box"));
	CreateBlock(&m_blockObject, D3DXVECTOR2(28, 8 + 26),	D3DXVECTOR2(13, 13), GetSingleton(TextureLoader)->GetTexture(L"Object_Box"));
}

void _2Stage::Update()
{
	if (!m_player->GetHealth())
	{
		GetSingleton(SceneManager)->ChangeScene(L"Ending");
		return;
	}



	if (m_timer < 3600)
	{
		if (m_timer % 90 == 0)
		{
			Enemy* enemy = new Enemy();
			enemy->Initialize(m_player, &m_enemyAttack, 1);
			enemy->SetPosition(D3DXVECTOR2(m_camera.GetPosition().x > 0 ? -700 : 700, 12));
			m_enemy.push_back(enemy);
		}
		if (m_timer % 360 == 0)
		{
			Enemy* enemy = new Enemy();
			enemy->Initialize(m_player, &m_enemyAttack, 2);
			enemy->SetPosition(D3DXVECTOR2(m_camera.GetPosition().x > 0 ? -510 : 510, 100));
			m_enemy.push_back(enemy);
		}
	}
	else
	{
		if (!m_warning && !m_apc)
		{
			m_warning = new Warning();
			m_warning->Initialize();
		}
		else if (!m_apc)
		{
			m_warning->Update();
			if (m_warning->PleaseDeleteThis())
			{
				m_apc = new APC();
				m_apc->Initialize(&m_enemyAttack, &m_effect, m_player);
				SAFE_RELEASE_DELETE(m_warning);
			}
		}

		if (m_timer % 120 == 0)
		{
			Enemy* enemy = new Enemy();
			enemy->Initialize(m_player, &m_enemyAttack, 1);
			enemy->SetPosition(D3DXVECTOR2(m_camera.GetPosition().x > 0 ? -700 : 700, 12));
			m_enemy.push_back(enemy);
		}
		if (m_timer % 600 == 0)
		{
			Enemy* enemy = new Enemy();
			enemy->Initialize(m_player, &m_enemyAttack, 2);
			enemy->SetPosition(D3DXVECTOR2(m_camera.GetPosition().x > 0 ? -510 : 510, 100));
			m_enemy.push_back(enemy);
		}
	}
	m_timer++;


	for (auto& iter : m_road.GetVertex())
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
		for (auto& iterE : m_enemy)
		{
			if (YPositionDeduction(result, iterE->GetPosition() - D3DXVECTOR2(0, iterE->GetAABB().GetSize().y), iterE->GetGravity()))
			{
				iterE->SetPosition(D3DXVECTOR2(iterE->GetPosition().x, result + iterE->GetAABB().GetSize().y));
				iterE->SetGravity(0);
			}
		}

		for (auto& iterE : m_playerAttack)
		{
			if (rand() % 4 == 0 && YPositionDeduction(result, iterE->GetPosition(), 1))
				iterE->Expire();
			if (iterE->GetPosition().y < -160)
				iterE->Expire();
		}
		for (auto& iterE : m_enemyAttack)
		{
			if (rand() % (iterE->IsFake() ? 3 : 5) == 0 && YPositionDeduction(result, iterE->GetPosition(), 1))
				iterE->Expire();
			if (iterE->GetPosition().y < -160)
				iterE->Expire();
		}
	}

	for (auto& iter : m_specialPlace.GetVertex())
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
	}



	m_player->Update();
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


	//APC
	if (m_apc)
	{
		m_apc->Update();
		if (m_apc->GetAABB().CalculateCollision(m_player->GetAABB()))
			m_player->Kill();

		if (!m_apc->GetHealth() && !(m_timer % 6) && m_apc->GetState() != 3)
		{
			GrenadeEffect* effect = new GrenadeEffect(m_apc->GetPosition() + D3DXVECTOR2(rand() % 250 - 125, rand() % 250 - 125));
			m_effect.push_back(effect);
		}
	}


	//Enemy
	for (auto iter = m_enemy.begin(); iter != m_enemy.end();)
	{
		(*iter)->Update();

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
		if ((*iter)->GetAABB().CalculateCollision(m_attackBlock->GetAABB()))
			(*iter)->Expire();

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

		if (m_apc && m_apc->GetAABB().CalculateCollision((*iter)->GetAABB()))
		{
			(*iter)->Expire();
			m_apc->Hit();
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

		if ((*iter)->GetPosition().y < 0)
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
				cout << "Hit" << endl;
				m_player->Hit();
			}
		}
		for (auto& iterB : m_blockObject)
		{
			if ((*iter)->GetAABB().CalculateCollision(iterB->GetAABB()))
				(*iter)->Expire();
		}
		if ((*iter)->GetAABB().CalculateCollision(m_attackBlock->GetAABB()))
			(*iter)->Expire();

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

		 if (cameraPos.x - m_camera.GetSize().x < -700) cameraPos.x = -700 + m_camera.GetSize().x;
	else if (cameraPos.x + m_camera.GetSize().x > +700) cameraPos.x = +700 - m_camera.GetSize().x;
	if (cameraPos.y - m_camera.GetSize().y < -12) cameraPos.y = -12 + m_camera.GetSize().y;

	m_camera.SetCameraTranslation(&D3DXVECTOR3(cameraPos.x, cameraPos.y, 0), &D3DXVECTOR3(800 * 0.5f, 450 * 0.5f, 100));
}

void _2Stage::Render()
{
	m_camera.InputCameraDevice(DEVICE);
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"BG0"), m_camera.GetPosition() + D3DXVECTOR3(-m_camera.GetSize().x, 0, 100), false, D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(0.6f, 0.6f));
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"BG0"), m_camera.GetPosition() + D3DXVECTOR3(0, 0, 100), false, D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(0.6f, 0.6f));
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"BG0"), m_camera.GetPosition() + D3DXVECTOR3(+m_camera.GetSize().x, 0, 100), false, D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(0.6f, 0.6f));
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"2Stage"), D3DXVECTOR3(0, 283 - 12, 50));


	for (auto& iter : m_enemyAttack)
		iter->Render();
	for (auto& iter : m_playerAttack)
		iter->Render();
	for (auto& iter : m_grenade)
		iter->Render();

	for (auto& iter : m_blockObject)
		iter->Render();

	for (auto& iter : m_enemy)
		iter->Render();

	for (auto& iter : m_item)
		iter->Render();

	if (m_apc)
		m_apc->Render();

	for (auto& iter : m_effect)
		iter->Render();
	m_player->Render();

	if (m_warning)
		m_warning->Render(m_camera.GetPosition());
	
	GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(L"UI_Player_Health"), D3DXVECTOR3(166.5 + 135, 14.5 + 30, 0), D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(1, 1), (float)m_player->GetHealth() / 30.);
	GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(L"UI_Player_Skill"),	D3DXVECTOR3(166.5 + 135, 10 + 58, 0), D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(1, 1), (float)m_player->GetSpecialAttackTime() / 600.);
	GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(L"UI_Player_Grenade"),D3DXVECTOR3(46.5 + 149, 35 + 90, 0));
	GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(std::wstring(L"Num") + std::to_wstring(m_player->GetGrenadeCount())), D3DXVECTOR3(46.5 + 149 + 20, 35 + 90 + 10, 0));
	GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(L"UI_Player"),		D3DXVECTOR3(225 + 20, 50 + 20, 0));

	if (m_apc)
	{
		GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(L"UI_Boss1_Health"), D3DXVECTOR3(1600 - 388 + 172 - 85 - 20, 23 + 9 + 20, 0), D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(1, 1), (float)m_apc->GetHealth() / 10.);
		GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(L"UI_Boss1_Frame"), D3DXVECTOR3(1600 - 244 - 20, 50 + 20, 0));
	}
}

void _2Stage::Release()
{
	for (auto& iter : m_enemy)			SAFE_RELEASE_DELETE(iter);
	for (auto& iter : m_grenade)		SAFE_RELEASE_DELETE(iter);
	for (auto& iter : m_playerAttack)	SAFE_RELEASE_DELETE(iter);
	for (auto& iter : m_enemyAttack)	SAFE_RELEASE_DELETE(iter);
	for (auto& iter : m_blockObject)	SAFE_RELEASE_DELETE(iter);
	for (auto& iter : m_item)			SAFE_RELEASE_DELETE(iter);
	for (auto& iter : m_effect)			SAFE_RELEASE_DELETE(iter);
	m_enemy.clear();
	m_grenade.clear();
	m_playerAttack.clear();
	m_enemyAttack.clear();
	m_blockObject.clear();
	m_item.clear();
	m_effect.clear();

	SAFE_RELEASE_DELETE(m_player);
	SAFE_RELEASE_DELETE(m_apc);
	SAFE_RELEASE_DELETE(m_attackBlock);
	SAFE_RELEASE_DELETE(m_warning);
}
