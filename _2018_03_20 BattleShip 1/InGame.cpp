#include "DXUT.h"
#include "InGame.h"

#include "Map.h"
#include "Ship.h"
#include "GameUI.h"

#include "Effect.h"
#include "ExplosionHit.h"
#include "ExplosionMiss.h"
#include "Marker.h"

#include "Missile.h"

#include "GameAI.h"

#include "Satellite.h"
constexpr UINT TURN_CHANGE_DELAY = 30;


Channel*  victory = nullptr;

InGame::InGame()
	: m_settingTime(true)

	, m_view(3)

	, m_victoryFrame(0)
	, m_victory(false)
	, m_playerWin(false)


	, m_playerTurn(true)
	, m_enemySeleteDelay(0)
	, m_turnChangeDelay(0)
	, m_scoreFont(nullptr)
	, m_turn(0)


	, m_uiPlayer(nullptr)
	, m_mapPlayer(nullptr)
	, m_mapEnemy(nullptr)
	, m_prevAttackHit(false)

	, m_uiEnemy(nullptr)
	, m_settingCursor(-1)
	, m_settingLookAtRight(false)

	, m_ai(nullptr)

	, m_bgm(nullptr)
{
	m_shipPlayer.fill(nullptr);
	m_shipEnemy.fill(nullptr);

	for (UINT i = 0; i < 12; i++)
		LoadTex(L"./Resource/effect/Explosion_" + std::to_wstring(i) + L".png", L"ExplosionHit" + std::to_wstring(i));
	for (UINT i = 0; i < 11; i++)
		LoadTex(L"./Resource/effect/Explosion_Failled_" + std::to_wstring(i) + L".png", L"ExplosionMiss" + std::to_wstring(i));
	//for (UINT i = 0; i < 13; i++)
	//	LoadTex(L"./Resource/effect/Missile_Effect_" + std::to_wstring(i) + L".png", L"MissileTail" + std::to_wstring(i));
	for (UINT i = 0; i < 9; i++)
		LoadTex(L"./Resource/temp/flameShot" + std::to_wstring(i) + L".png", L"MissileTail" + std::to_wstring(i));


	LoadTex(L"./Resource/ui/playerTurn.png", L"playerTurn");
	LoadTex(L"./Resource/ui/enemyTurn.png", L"enemyTurn");
	LoadTex(L"./Resource/ui/Score.png", L"Score");
	LoadTex(L"./Resource/ui/HighScore.png", L"HighScore");

	ASELoad(L"./Resource/model", L"Missile.ase", L"missile");
	ASELoad(L"./Resource/model", L"No.ase",		L"marker_fail");
	ASELoad(L"./Resource/model", L"Check.ase", L"marker_true");
	ASELoad(L"./Resource/model", L"Satellite.ase",	L"Satellite");


	for (UINT i = 0; i < 4; i++)
		LoadTex(L"./Resource/ui/Victory_" + std::to_wstring(i) + L".png", L"Victory" + std::to_wstring(i));
	for (UINT i = 0; i < 4; i++)
		LoadTex(L"./Resource/ui/Lose_" + std::to_wstring(i) + L".png", L"Lose" + std::to_wstring(i));
}
InGame::~InGame()
{
}



void InGame::Initialize()
{
	m_prevCursorPos.x = m_prevCursorPos.y = -1;

	m_bgm = SingletonInstance(SoundManager)->Play(L"BGM_InGame");
	SingletonInstance(Score)->InsertScore(-100000000);

	m_view = 3;

	m_settingTime = true;
	m_settingCursor = -1;
	m_settingLookAtRight = false;
	
	m_victory = false;
	m_playerWin = false;
	m_victoryFrame = 0;
	m_prevAttackHit = false;
	m_turn = 1;
	m_playerTurn = true;

	m_enemySeleteDelay = 0;
	m_turnChangeDelay = 0;


	ASELoad(L"./Resource/model", L"Ship2.ase", L"x2Ship");
	ASELoad(L"./Resource/model", L"Ship3.ase", L"x3Ship");
	ASELoad(L"./Resource/model", L"Ship4.ase", L"x4Ship");


	m_uiPlayer = new GameUI();
	m_uiEnemy = new GameUI(true);


	constexpr float distance = 2;
	m_mapPlayer = new Map(D3DXVECTOR3(-5 - distance, 0, 0));
	m_mapEnemy = new Map(D3DXVECTOR3(5 + distance, 0, 0));

	m_itemPlayer.fill(true);
	m_itemEnemy.fill(true);

	m_shipSetting.fill(false);
	m_shipPlayer[0] = new Ship(2);
	m_shipPlayer[1] = new Ship(2);
	m_shipPlayer[2] = new Ship(3);
	m_shipPlayer[3] = new Ship(3);
	m_shipPlayer[4] = new Ship(4);

	m_shipEnemy[0] = new Ship(2);
	m_shipEnemy[1] = new Ship(2);
	m_shipEnemy[2] = new Ship(3);
	m_shipEnemy[3] = new Ship(3);
	m_shipEnemy[4] = new Ship(4);


	m_marker.push_back(new Satellite());

	srand(GetTickCount());
	/* AI ShipSetting */ {
		for (UINT i = 0; i < 5; i++)
		{
			Ship* ship = m_shipEnemy[i];
			UINT size = ship->GetSize();

			bool lookAtRight;
			POINT pos;
			while (true)
			{
				lookAtRight = rand() & 0b1;
				pos.x = rand() % (10 - (lookAtRight ? size : 1));
				pos.y = rand() % (10 - (lookAtRight ? 1 : size));

				m_isCanPlace = true; {
					POINT cPos = pos;
					auto& cPosRef = lookAtRight ? cPos.x : cPos.y;
					for (UINT i = 0; i < size; i++)
					{
						if (m_mapEnemy->GetTileState(cPos.x, cPos.y))
						{
							m_isCanPlace = false;
							break;
						}
						cPosRef++;
					}
				}
				if (m_isCanPlace)
					break;
			}

			POINT cPos = pos;
			auto& cPosRef = lookAtRight ? cPos.x : cPos.y;
			for (UINT i = 0; i < size; i++)
			{
				m_mapEnemy->RegistShip(cPos.x, cPos.y, ship);
				cPosRef++;
			}

			ship->Assign(pos, lookAtRight);
		}
	}

	m_ai = new GameAI();


	D3DXCreateFontW(DEVICE, 60, 0, FW_DONTCARE, 1, false, DEFAULT_CHARSET, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"HYShortSamul-Medium", &m_scoreFont);
}

void InGame::Update()
{
	if (!m_bgm->IsPlaying())
		m_bgm->Play();

	if (SingletonInstance(Fader)->Update() && m_victory && SingletonInstance(Fader)->IsIn())
	{
		SingletonInstance(Score)->ScoreSave();

		SingletonInstance(Fader)->Create(false);
		SingletonInstance(Fader)->SetStay(30);
		SingletonInstance(SceneManager)->ChangeScene(L"MainMenu");
		if (victory)
			victory->Stop();
		return;
	}
	
	if (SingletonInstance(InputManager)->IsKeyDown('V'))
	{
		if (m_view)
			m_view--;
		else
			m_view = 3;
	}

	
	if (m_victory)
	{
		m_victoryFrame++;
		if (SingletonInstance(InputManager)->IsKeyDown(VK_SPACE) && !SingletonInstance(Fader)->IsRunning())
			SingletonInstance(Fader)->Create(true);
	}
	else
	{
		m_settingTime ?
			SettingTimeUpdate() :
			RunTimeUpdate();
	}

	/* Effect */
	for (auto& iter = m_effect.begin(); iter != m_effect.end();)
	{
		(*iter)->Update();
		if ((*iter)->IsExpired())
		{
			delete *iter;
			iter = m_effect.erase(iter);
		}
		else
			iter++;
	}
	for (auto& iter = m_marker.begin(); iter != m_marker.end();)
	{
		(*iter)->Update();
		if ((*iter)->IsExpired())
		{
			delete *iter;
			iter = m_marker.erase(iter);
		}
		else
			iter++;
	}

	/* Missile */
	{
		for (auto& iter : m_missile)
		{
			iter->Update(m_effect);
			D3DXVECTOR3 pos	= iter->GetPos();
			bool byPlayer	= iter->IsByPlayer();
			bool diving		= iter->IsDiving();

			auto GetOffset = []()->D3DXVECTOR3
			{
				constexpr FLOAT RANGE = 25;
				D3DXVECTOR3 offset(0, 0, rand() % 1000 / 1000. * RANGE);
				D3DXMATRIX y;
				D3DXMatrixRotationY(&y, D3DXToRadian(rand() % 3600 / 10.));
				return *D3DXVec3TransformNormal(&offset, &offset, &y);
			};
			auto RandomDir = []()->D3DXVECTOR3
			{
				constexpr FLOAT OFFSET_ANGLE = 15;
				D3DXVECTOR3 dir(0, -1, 0);
				D3DXMATRIX xRM, yRM;
				D3DXMatrixRotationX(&xRM, D3DXToRadian(rand() % (int)(OFFSET_ANGLE * 100) / 100. ));
				D3DXMatrixRotationY(&yRM, D3DXToRadian(rand() % 3600 / 10.));
				D3DXVec3TransformNormal(&dir, &dir, &xRM);
				D3DXVec3TransformNormal(&dir, &dir, &yRM);
				return dir;
			};

			D3DXVECTOR3 nextPos = (byPlayer ? m_mapEnemy : m_mapPlayer)->GetMapPos();
			if (!diving)
			{
				if (pos.y > 40)
				{
					nextPos.y += 40;
					nextPos += GetOffset();
					iter->SetPos(nextPos);
					iter->SetDir(RandomDir());
					iter->Diving();
				}
			}
			else
			{
				if (pos.y < 0)
					iter->Expire();
			}
		}
		/* erase */
		for (auto& iter = m_missile.begin(); iter != m_missile.end();)
		{
			if ((*iter)->IsExpired())
			{
				m_effect.push_back((*iter)->isHit() ?
					(Effect*)new ExplosionHit((*iter)->GetPos()) :
					(Effect*)new ExplosionMiss((*iter)->GetPos()));
				
				(*iter)->isHit() ?
					SingletonInstance(SoundManager)->Play(L"Hit") :
					SingletonInstance(SoundManager)->Play(L"Miss");

				delete *iter;
				iter = m_missile.erase(iter);
			}
			else
				iter++;
		}
	}

	m_uiPlayer->Update();
	m_uiEnemy->Update();
}

void InGame::Render()
{
	DEVICE->SetRenderState(D3DRS_ZENABLE, false);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, false);
	DEVICE->SetMaterial(&WHITEMTRL);
	SingletonInstance(Skybox)->Render();
	DEVICE->SetRenderState(D3DRS_ZENABLE, true);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);

	for (auto& iter : m_missile)
	{
		if (iter->IsByPlayer() || iter->IsDiving())
			iter->Render();
	}

	//else
	//{
		m_settingTime ?
			SettingTimeRender() :
			RunTimeRender();
	//}

	DEVICE->SetRenderState(D3DRS_ZENABLE, false);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, false);
	for (auto& iter : m_effect)	iter->Render();
	if (m_view / 2)
		for (auto& iter : m_marker)	iter->Render();
	DEVICE->SetRenderState(D3DRS_ZENABLE, true);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);


	if (m_victory)
	{
		SPRITE->Begin(D3DXSPRITE_ALPHABLEND);
		auto DrawVictory = [&, this](const float& size, const float& angle, const UINT& index, const bool& playerWin)
		{
			Texture* tex = GetTex((playerWin ? L"Victory" : L"Lose") + std::to_wstring(index));
			D3DXMATRIX sm, rm, tm;
			D3DXMatrixScaling(&sm, size, size, size);
			D3DXMatrixRotationZ(&rm, D3DXToRadian(-angle));
			D3DXMatrixTranslation(&tm, 800, 450, 0);
			SPRITE->SetTransform(&(sm * rm * tm));
			SPRITE->Draw(tex->tex, nullptr, &D3DXVECTOR3(tex->info.Width * 0.5f, tex->info.Height * 0.5f, 0), nullptr, D3DXCOLOR(1, 1, 1, 1));
		};

		for (UINT i = 0; i < 4; i++)
		{
			float size = (float)((i + 1) * 15) - m_victoryFrame / 2;
			if (size < 1)
				size = 1;
			float rotation = m_victoryFrame * i * 1.5;
			DrawVictory(size, rotation, i, m_playerWin);
		}

		SPRITE->End();
	}
	
	SingletonInstance(Fader)->Render();
}

void InGame::Release()
{
	SAFE_DELETE( m_mapPlayer );
	SAFE_DELETE( m_mapEnemy );
	for (auto &iter : m_shipPlayer)	SAFE_DELETE(iter);
	for (auto &iter : m_shipEnemy)	SAFE_DELETE(iter);
	for (auto &iter : m_effect)		SAFE_DELETE(iter);	m_effect.clear();
	for (auto &iter : m_marker)		SAFE_DELETE(iter);	m_marker.clear();
	for (auto &iter : m_missile)	SAFE_DELETE(iter);	m_missile.clear();

	SAFE_DELETE( m_ai );
	SAFE_RELEASE( m_scoreFont );

	m_bgm->Stop();
}



void InGame::SettingTimeUpdate()
{
	auto Button = [](const D3DXVECTOR2& lt, const D3DXVECTOR2& rb)
	{
		POINT mPos = SingletonInstance(InputManager)->GetMousePos();
		return SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON) &&
			lt.x < mPos.x &&
			lt.y < mPos.y &&
			mPos.x < rb.x &&
			mPos.y < rb.y;
	};

	bool ㄴㅌㄱㄷㅇㅊㄹ호ㅓㅠ = false;
	/* Btn */ {
			 if (Button(	D3DXVECTOR2(11, 296) + D3DXVECTOR2(25,			21)			,	D3DXVECTOR2(11, 296) +	D3DXVECTOR2(25,			21)			+ D3DXVECTOR2(195, 98) )) m_settingCursor = m_shipSetting[0] ? -1 : 0;
		else if (Button(	D3DXVECTOR2(11, 296) + D3DXVECTOR2(25 + 211,	21)			,	D3DXVECTOR2(11, 296) +	D3DXVECTOR2(25 + 211,	21)			+ D3DXVECTOR2(195, 98) )) m_settingCursor = m_shipSetting[1] ? -1 : 1;
		else if (Button(	D3DXVECTOR2(11, 296) + D3DXVECTOR2(25,			21 + 110)	,	D3DXVECTOR2(11, 296) +	D3DXVECTOR2(25,			21 + 110)	+ D3DXVECTOR2(195, 98) )) m_settingCursor = m_shipSetting[2] ? -1 : 2;
		else if (Button(	D3DXVECTOR2(11, 296) + D3DXVECTOR2(25 + 211,	21 + 110)	,	D3DXVECTOR2(11, 296) +	D3DXVECTOR2(25 + 211,	21 + 110)	+ D3DXVECTOR2(195, 98) )) m_settingCursor = m_shipSetting[3] ? -1 : 3;
		else if (Button(	D3DXVECTOR2(11, 296) + D3DXVECTOR2(25,			21 + 220)	,	D3DXVECTOR2(11, 296) +	D3DXVECTOR2(25,			21 + 220)	+ D3DXVECTOR2(195, 98) )) m_settingCursor = m_shipSetting[4] ? -1 : 4;
		else ㄴㅌㄱㄷㅇㅊㄹ호ㅓㅠ = true;
	}

	if (!ㄴㅌㄱㄷㅇㅊㄹ호ㅓㅠ)
	{
		SingletonInstance(SoundManager)->Play(L"SeleteShip");
		return;
	}

	if (SingletonInstance(InputManager)->IsKeyDown(VK_RBUTTON))	m_settingLookAtRight = !m_settingLookAtRight;


	if (m_settingCursor != -1)
	{
		UINT size = m_shipPlayer[m_settingCursor]->GetSize();
		if (m_mapPlayer->GetCursorTile(m_settingShipPos))
		{
			if (m_prevCursorPos.x != m_settingShipPos.x ||
				m_prevCursorPos.y != m_settingShipPos.y)
				SingletonInstance(SoundManager)->Play(L"SeleteTile");

			if (m_settingLookAtRight)
			{
				if (m_settingShipPos.x > 10 - size)
					m_settingShipPos.x = 10 - size;
			}
			else
			{
				if (m_settingShipPos.y > 10 - size)
					m_settingShipPos.y = 10 - size;
			}


			m_isCanPlace = true; {
				POINT cPos = m_settingShipPos;
				auto& cPosRef = m_settingLookAtRight ? cPos.x : cPos.y;
				for (UINT i = 0; i < size; i++)
				{
					if (m_mapPlayer->GetTileState(cPos.x, cPos.y))
					{
						m_isCanPlace = false;
						break;
					}
					cPosRef++;
				}
			}
			if (m_isCanPlace && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON))
			{
				POINT cPos = m_settingShipPos;
				auto& cPosRef = m_settingLookAtRight ? cPos.x : cPos.y;
				for (UINT i = 0; i < size; i++)
				{
					m_mapPlayer->RegistShip(cPos.x, cPos.y, m_shipPlayer[m_settingCursor]);
					cPosRef++;
				}

				m_shipPlayer[m_settingCursor]->Assign(m_settingShipPos, m_settingLookAtRight);

				m_settingLookAtRight = false;
				m_settingShipPos.x = m_settingShipPos.y = -1;
				m_shipSetting[m_settingCursor] = true;
				m_uiPlayer->ShipSetted(m_settingCursor);

				m_settingCursor = -1;
			}
		}
		else
			m_settingShipPos.x = m_settingShipPos.y = -1;

		m_prevCursorPos = m_settingShipPos;
	}


	/* Check  */
	for (UINT i = 0; i < 5; i++)
	{
		if (m_shipSetting[i])
		{
			if (i == 4)
				m_settingTime = false;
		}
		else
			break;
	}
}

void InGame::RunTimeUpdate()
{
	if (m_turnChangeDelay)
		m_turnChangeDelay--;
	else if (!m_missile.size())
	{
		auto Attack = [this](const UINT& count, const D3DXVECTOR3& pos, const D3DXVECTOR3& objectivePos, const D3DXVECTOR3& terminal, const bool& hit, const bool& attackByPlayer)
				{
			SingletonInstance(SoundManager)->Play(L"Missile");
			for (UINT i = 0; i < count; i++)
						{
							D3DXVECTOR3 dir;
							D3DXMATRIX rot;
							D3DXMatrixRotationY(&rot, D3DXToRadian(360 * ((float)i / count)));
							D3DXVec3TransformNormal(&dir, &D3DXVECTOR3(0, 0, 1), &rot);

							m_missile.push_back( new Missile(pos, dir, objectivePos, terminal, attackByPlayer, hit) );
						}
				};
			
		if (m_playerTurn)
		{
			POINT clickTile;
			if (SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON) &&
				m_mapEnemy->GetCursorTile(clickTile) &&
				!m_mapEnemy->GetTile(clickTile.x, clickTile.y).second)
			{
				auto& tile = m_mapEnemy->GetTile(clickTile.x, clickTile.y);
				tile.second = true;
				D3DXVECTOR3 worldPos = D3DXVECTOR3(clickTile.x - 4.5, 0, 4.5 - clickTile.y) + m_mapEnemy->GetMapPos();

				D3DXVECTOR3 missilePos; {
					Ship* ship = nullptr;
					do
					{
						ship = m_shipPlayer[rand() % 5];
					} while (!ship->GetLife());
					missilePos = m_mapPlayer->GetMapPos() + D3DXVECTOR3(ship->GetPos().x - 4.5, 0, 4.5 - ship->GetPos().y);
				}
				D3DXVECTOR3 missileObjPos(missilePos + D3DXVECTOR3(0, 40, 0));
				
				bool isHit = tile.first;
				if (isHit)
					tile.first->Hit();
				Attack(8, missilePos, missileObjPos, worldPos, isHit, true);
				m_marker.push_back(new Marker(worldPos, isHit));

				if (isHit)
					SingletonInstance(Score)->InsertScore(m_prevAttackHit ? 150 : 100);

				m_prevAttackHit = isHit;

				m_playerTurn = false;
				m_enemySeleteDelay = rand() % 30 + 30;
				m_turnChangeDelay = TURN_CHANGE_DELAY;
			}
		}
		else
		{
			if (m_enemySeleteDelay)
				m_enemySeleteDelay--;
			else
			{
				POINT aiOutput;
				if (m_ai->Output(aiOutput))
				{
					bool canAttack = !m_mapPlayer->GetTile(aiOutput.x, aiOutput.y).second;
					bool isHit = false;
					if (canAttack)
					{
						auto& tile = m_mapPlayer->GetTile(aiOutput.x, aiOutput.y);
						tile.second = true;
						D3DXVECTOR3 worldPos = D3DXVECTOR3(aiOutput.x - 4.5, 0, 4.5 - aiOutput.y) + m_mapPlayer->GetMapPos();

						D3DXVECTOR3 missilePos; {
							Ship* ship = nullptr;
							do
							{
								ship = m_shipEnemy[rand() % 5];
							} while (!ship->GetLife());
							missilePos = m_mapEnemy->GetMapPos() + D3DXVECTOR3(ship->GetPos().x - 4.5, 0, 4.5 - ship->GetPos().y);
						}
						D3DXVECTOR3 missileObjPos(missilePos + D3DXVECTOR3(0, 45, 0));
						isHit = tile.first;
						if (isHit)
							tile.first->Hit();
						Attack(8, missilePos, missileObjPos, worldPos, isHit, false);
						m_marker.push_back(new Marker(worldPos, isHit));

						if (isHit)
							SingletonInstance(Score)->InsertScore(-5);
						SingletonInstance(Score)->InsertScore(-1);

						m_turn++;
						m_playerTurn = true;
						m_turnChangeDelay = TURN_CHANGE_DELAY;
					}
					m_ai->Input(canAttack, isHit);
				}
			}
		}
	}

	bool enemyWin = true;
	for (auto& iter : m_shipPlayer)
	{
		if (iter->GetLife())
		{
			enemyWin = false;
			break;
		}
	}
	if (enemyWin)
	{
		m_playerWin = false;
		m_victory = true;
	}

	bool playerWin = true;
	for (auto& iter : m_shipEnemy)
	{
		if (iter->GetLife())
		{
			playerWin = false;
			break;
		}
	}
	if (playerWin)
	{
		m_playerWin = true;
		m_victory = true;
		victory = SingletonInstance(SoundManager)->Play(L"Victory");
	}
}


void InGame::SettingTimeRender()
{
	if (m_settingCursor != -1 && m_settingShipPos.x != -1)
	{
		POINT cPos = m_settingShipPos;
		auto& cPosRef = m_settingLookAtRight ? cPos.x : cPos.y;
		for (UINT i = 0; i < m_shipPlayer[m_settingCursor]->GetSize(); i++)
		{
			m_mapPlayer->DrawTile(cPos.x, cPos.y, D3DXVECTOR3(0, 0.1, 0), GetTex(m_isCanPlace ? L"Tile_Green" : L"Tile_Red"));
			cPosRef++;
		}
	}

	D3DXMATRIX mapTM;
	D3DXMatrixTranslation(&mapTM, m_mapPlayer->GetMapPos().x, m_mapPlayer->GetMapPos().y, m_mapPlayer->GetMapPos().z);
	for (UINT i = 0; i < 5; i++)
	{
		if (m_shipSetting[i])
			m_shipPlayer[i]->Render(mapTM);
	}

	if (m_view & 0b1) {
		m_mapPlayer->Render(false);
		m_mapEnemy->Render(false);
	}


	m_uiPlayer->Render(m_shipPlayer, D3DXVECTOR2(11, 296), false, m_settingCursor);
	//m_uiEnemy->Render(m_shipEnemy, D3DXVECTOR2(1600 - 282, 7));
}

void InGame::RunTimeRender()
{
	D3DXMATRIX mapTM;
	D3DXMatrixTranslation(&mapTM, m_mapPlayer->GetMapPos().x, m_mapPlayer->GetMapPos().y, m_mapPlayer->GetMapPos().z);
	for (UINT i = 0; i < 5; i++)
	{
		if (m_shipPlayer[i]->GetLife())
			m_shipPlayer[i]->Render(mapTM);
	}

	if (SingletonInstance(InputManager)->IsKeyPressed('K'))
	{
		D3DXMatrixTranslation(&mapTM, m_mapEnemy->GetMapPos().x, m_mapEnemy->GetMapPos().y, m_mapEnemy->GetMapPos().z);
		for (UINT i = 0; i < 5; i++)
		{
			if (m_shipEnemy[i]->GetLife())
				m_shipEnemy[i]->Render(mapTM);
		}
	}

	float time = 1 - ((float)m_turnChangeDelay / TURN_CHANGE_DELAY);
	float yForX = sqrt(1 - pow(1 - time, 2)); // 0 to 1, slow to fast

	if (m_view & 0b1) {
		m_mapPlayer->Render(true);
		m_mapEnemy->Render(true);
	}


	/* Draw UI */ {
		constexpr FLOAT MOVEMENT = 500;

		D3DXVECTOR2 playerUIPos; {
			playerUIPos = D3DXVECTOR2(11, 296);
			m_playerTurn ?
				playerUIPos.x -= (1 - yForX) * MOVEMENT :
				playerUIPos.x -= yForX * MOVEMENT;
		}
		D3DXVECTOR2 enemyUIPos; {
			enemyUIPos = D3DXVECTOR2(1591 - 486, 296);
			m_playerTurn ?
				enemyUIPos.x += yForX * MOVEMENT :
				enemyUIPos.x += (1 - yForX) * MOVEMENT;
		}
		m_uiPlayer->Render(m_shipPlayer, playerUIPos);
		m_uiEnemy->Render(m_shipEnemy, enemyUIPos);
	}

	SPRITE->Begin(D3DXSPRITE_ALPHABLEND);
	/* Draw Turn */ {

		float scale = 1; {
			constexpr FLOAT SIZE = 0.2;
			if (time < 0.4)
			{
				if (time > 0.2)
					scale += (0.4 - time) * 5 * SIZE;
				else
					scale += time * 5 * SIZE;
			}
		}
		D3DXMATRIX sm, tm;
		D3DXMatrixScaling(&sm, scale, scale, scale);
		D3DXMatrixTranslation(&tm, 1600 / 2, 10, 0);
		SPRITE->SetTransform(&(sm * tm));
		
		Texture* tex = GetTex(m_playerTurn ? L"playerTurn" : L"enemyTurn");
		SPRITE->Draw(tex->tex, nullptr, &D3DXVECTOR3(tex->info.Width * 0.5f, 0, 0), nullptr, D3DXCOLOR(1, 1, 1, 1));
	}

	/* Score */ {
		D3DXMATRIX iden;
		D3DXMatrixIdentity(&iden);
		SPRITE->SetTransform(&iden);

		SPRITE->Draw(GetTex(L"Score")->tex, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, 1));
		Texture* tex = GetTex(L"HighScore");
		SPRITE->Draw(tex->tex, nullptr, &D3DXVECTOR3(tex->info.Width, 0, 0), &D3DXVECTOR3(1600, 0, 0), D3DXCOLOR(1, 1, 1, 1));

		RECT rc;
		SetRect(&rc, 440, 70, 440, 70);
		m_scoreFont->DrawTextW(SPRITE, (std::to_wstring(SingletonInstance(Score)->GetScore()) + L" (턴: " + std::to_wstring(m_turn) + L")").data(), -1, &rc, DT_NOCLIP | DT_VCENTER | DT_RIGHT, D3DXCOLOR(41 / 255., 235 / 255., 1, 1));
		SetRect(&rc, 1600 - 200, 70, 1600 - 200, 70);
		m_scoreFont->DrawTextW(SPRITE, std::to_wstring(SingletonInstance(Score)->GetHighScore()).data(), -1, &rc, DT_NOCLIP | DT_VCENTER | DT_RIGHT, D3DXCOLOR(41 / 255., 235 / 255., 1, 1));
	}
	SPRITE->End();
}