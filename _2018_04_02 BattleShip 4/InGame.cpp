#include "DXUT.h"
#include "InGame.h"

#include "Map.h"
#include "Ship.h"
#include "AIPlayer.h"
#include "GameUI.h"
#include "GameCam.h"

#include "Lockoner.h"
#include "Missile.h"
#include "Scanner.h"

#include "Effect.h"
#include "LockonEffect.h"
#include "MissileTail.h"
#include "MissileAttackEffect.h"
#include "ShipSmoke.h"


InGame::InGame()
{
}
InGame::~InGame()
{
}



void InGame::Initialize()
{
	TextureLoad(L"./Resource/map/pMapFrame.png", L"pMapFrame");
	TextureLoad(L"./Resource/map/eMapFrame.png", L"eMapFrame");

	TextureLoad(L"./Resource/map/Tile_Default.png"	, L"Tile");
	TextureLoad(L"./Resource/map/Tile_Hit.png"		, L"TileT");
	TextureLoad(L"./Resource/map/Tile_Miss.png"		, L"TileF");
	TextureLoad(L"./Resource/map/Tile_Select.png"	, L"TileS");
	for (UINT i = 0; i < 10; i++)
		TextureLoad(L"./Resource/map/Click_" + std::to_wstring(i) + L".PNG", L"Lockon" + std::to_wstring(i));
	TextureLoad(L"./Resource/map/Scanner0.png", L"Scanner0");
	TextureLoad(L"./Resource/map/Scanner1.png", L"Scanner1");


	ASELoad(L"./Resource/model", L"SkySphere.ase", L"SkySphere");

	ASELoad(L"./Resource/model", L"T2.ase", L"ShipX2");
	ASELoad(L"./Resource/model", L"T3.ase", L"ShipX3");
	ASELoad(L"./Resource/model", L"T4.ase", L"ShipX4");

	ASELoad(L"./Resource/model", L"Missile.ase", L"Missile");


	TextureLoad(L"./Resource/ui/player/frame.PNG", L"pFrame");
	TextureLoad(L"./Resource/ui/enemy/frame.PNG", L"eFrame");

	TextureLoad(L"./Resource/ui/player/x2.PNG",	L"pShipX2");
	TextureLoad(L"./Resource/ui/player/x3.PNG",	L"pShipX3");
	TextureLoad(L"./Resource/ui/player/x4.PNG",	L"pShipX4");
	TextureLoad(L"./Resource/ui/enemy/x2.PNG",	L"eShipX2");
	TextureLoad(L"./Resource/ui/enemy/x3.PNG",	L"eShipX3");
	TextureLoad(L"./Resource/ui/enemy/x4.PNG",	L"eShipX4");
	for (UINT i = 0; i < 4; i++)
	{
		TextureLoad(L"./Resource/ui/player/item"	+ std::to_wstring(i) + L"T.PNG", L"pItem" + std::to_wstring(i));
		TextureLoad(L"./Resource/ui/enemy/item"	+ std::to_wstring(i) + L"T.PNG", L"eItem" + std::to_wstring(i));
	}
	TextureLoad(L"./Resource/ui/player/randomPlaceT.PNG",		L"RandomPlace");
	TextureLoad(L"./Resource/ui/player/resetT.PNG",				L"Reset");
	TextureLoad(L"./Resource/ui/player/startT.PNG",				L"Start");

	TextureLoad(L"./Resource/ui/player/randomPlaceText.PNG",	L"RandomPlaceText");
	TextureLoad(L"./Resource/ui/player/resetText.PNG",			L"ResetText");
	TextureLoad(L"./Resource/ui/player/startText.PNG",			L"StartText");
	TextureLoad(L"./Resource/ui/player/skillCancel.PNG",		L"SkillCancelText");

	for (UINT i = 0; i < 3; i++)
	{
		TextureLoad(L"./Resource/ui/x2Noise" + std::to_wstring(i) + L".PNG", L"Noise2" + std::to_wstring(i));
		TextureLoad(L"./Resource/ui/x3Noise" + std::to_wstring(i) + L".PNG", L"Noise3" + std::to_wstring(i));
		TextureLoad(L"./Resource/ui/x4Noise" + std::to_wstring(i) + L".PNG", L"Noise4" + std::to_wstring(i));
	}
	
	TextureLoad(L"./Resource/ui/cam/Ui_Auto.PNG",		L"CamA");
	TextureLoad(L"./Resource/ui/cam/Ui_Auto2.PNG",		L"CamBoxA");
	TextureLoad(L"./Resource/ui/cam/Ui_Playable.PNG",	L"CamP");
	TextureLoad(L"./Resource/ui/cam/Ui_Playable2.PNG",	L"CamBoxP");
	TextureLoad(L"./Resource/ui/cam/Ui_Camera.PNG",		L"CamFrame");

	TextureLoad(L"./Resource/ui/Score.PNG", L"ScoreFrame");

	TextureLoad(L"./Resource/ui/Ally_Turn.PNG", L"pTurn");
	TextureLoad(L"./Resource/ui/Enemy_Turn.PNG", L"eTurn");
	TextureLoad(L"./Resource/ui/Victoru.PNG",  L"pWin");
	TextureLoad(L"./Resource/ui/Defeat.PNG", L"eWin");





	for (UINT i = 0; i < 9; i++)
		TextureLoad(L"./Resource/effect/missileTail/" + std::to_wstring(i) + L".PNG", L"MissileTail" + std::to_wstring(i));
	for (UINT i = 0; i < 12; i++)
		TextureLoad(L"./Resource/effect/missileAttack/" + std::to_wstring(i) + L".PNG", L"MissileAttack" + std::to_wstring(i));
	for (UINT i = 0; i < 12; i++)
		TextureLoad(L"./Resource/effect/shipSmoke/" + std::to_wstring(i) + L".PNG", L"ShipSmoke" + std::to_wstring(i));
	


	m_isWar = false;
	m_win = false;
	m_pWin = false;
	m_winFrame = 0;


	m_playerMap = new Map();
	m_playerShip[0] = new Ship(2);
	m_playerShip[1] = new Ship(2);
	m_playerShip[2] = new Ship(3);
	m_playerShip[3] = new Ship(3);
	m_playerShip[4] = new Ship(4);
	m_enemyMap = new Map();
	m_enemyShip[0] = new Ship(2);
	m_enemyShip[1] = new Ship(2);
	m_enemyShip[2] = new Ship(3);
	m_enemyShip[3] = new Ship(3);
	m_enemyShip[4] = new Ship(4);
	RandomPlaceShip(false);

	m_aiPlayer = new AIPlayer();
	m_playerUI = new GameUI(true);
	m_enemyUI = new GameUI(false);
	m_gameCam = new GameCam();


	m_cursor.x = m_cursor.y = -1;
	m_cursorLookAtY = false;
	m_cursorInPlayerMap = false;



	m_selectedShip = -1;
	m_isShipPlaced.fill(false);
	m_isCanPlace = false;

	m_toolSelecting = -1;



	m_isPlayerTurn = true;
	m_isReadyToAttack = true;
	m_turnCount = 1;

	m_itemSelecting = -1;
	m_selectedItem = -1;
	m_isPlayerItemUsed.fill(false);
	m_isEnemyItemUsed.fill(false);

	m_scanner = nullptr;
}

void InGame::Update()
{
	SingletonInstance(SkySphere)->Update();

	if (m_win)
		m_winFrame++;

	if (!m_isWar)
	{
		//커서
		m_cursorInPlayerMap = m_playerMap->GetCursor(m_cursor);
		if (SingletonInstance(InputManager)->IsKeyDown(VK_RBUTTON))	m_cursorLookAtY = !m_cursorLookAtY;

		//배 선택
		auto ShipBtnClicked = [](const D3DXVECTOR2& pos, const D3DXVECTOR2& size)->bool
		{
			POINT mPos = SingletonInstance(InputManager)->GetMousePos();
			return
				SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON) &&
				pos.x < mPos.x &&
				pos.y < mPos.y &&
				mPos.x < pos.x + size.x &&
				mPos.y < pos.y + size.y;
		};
		if (ShipBtnClicked(D3DXVECTOR2(41, 256),	D3DXVECTOR2(123, 110)) || SingletonInstance(InputManager)->IsKeyDown('1'))	m_selectedShip = m_isShipPlaced[0] ? -1 : 0;
		if (ShipBtnClicked(D3DXVECTOR2(179, 256),	D3DXVECTOR2(123, 110)) || SingletonInstance(InputManager)->IsKeyDown('2'))	m_selectedShip = m_isShipPlaced[1] ? -1 : 1;
		if (ShipBtnClicked(D3DXVECTOR2(41, 371),	D3DXVECTOR2(188, 110)) || SingletonInstance(InputManager)->IsKeyDown('3'))	m_selectedShip = m_isShipPlaced[2] ? -1 : 2;
		if (ShipBtnClicked(D3DXVECTOR2(41, 482),	D3DXVECTOR2(188, 110)) || SingletonInstance(InputManager)->IsKeyDown('4'))	m_selectedShip = m_isShipPlaced[3] ? -1 : 3;
		if (ShipBtnClicked(D3DXVECTOR2(41, 595),	D3DXVECTOR2(257, 110)) || SingletonInstance(InputManager)->IsKeyDown('5'))	m_selectedShip = m_isShipPlaced[4] ? -1 : 4;

		//커서 보정
		if (m_cursor.x != -1 && m_selectedShip != -1)
		{
			auto& scailer = (m_cursorLookAtY ? m_cursor.y : m_cursor.x);
			if (scailer > 10 - m_playerShip[m_selectedShip]->GetSize())
				scailer = 10 - m_playerShip[m_selectedShip]->GetSize();
		}


		
		//특수 커서
		auto CursorInToolButton = [](const D3DXVECTOR2& pos, const float& radius)->bool
		{
			POINT mPos = SingletonInstance(InputManager)->GetMousePos();
			return D3DXVec2Length(&(pos - D3DXVECTOR2(mPos.x, mPos.y))) < radius;
		};

		m_toolSelecting = -1;
		if (CursorInToolButton(D3DXVECTOR2(98 + 66 * 0, 822), 27))	m_toolSelecting = 0;
		if (CursorInToolButton(D3DXVECTOR2(98 + 66 * 1, 822), 27))	m_toolSelecting = 1;
		if (CursorInToolButton(D3DXVECTOR2(98 + 66 * 2, 822), 27))	m_toolSelecting = 2;



		//배 설치
		if (m_cursor.x != -1 && m_selectedShip != -1)
		{
			m_isCanPlace = CanPlaceShip(m_playerShip[m_selectedShip], m_cursor, m_cursorLookAtY, true);
			if (m_isCanPlace &&
				SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON))
			{
				PlaceShip(m_playerShip[m_selectedShip], m_cursor, m_cursorLookAtY, true);
				m_isShipPlaced[m_selectedShip] = true;
				m_selectedShip = -1;
			}
		}

		//배 랜덤
		if (SingletonInstance(InputManager)->IsKeyDown('A') ||
			(m_toolSelecting == 0 && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)))
			RandomPlaceShip(true);

		//배 리셋
		if (SingletonInstance(InputManager)->IsKeyDown('R') ||
			(m_toolSelecting == 1 && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)))
			ResetPlace(true);

		//게임 시작
		if (SingletonInstance(InputManager)->IsKeyDown('S') ||
			(m_toolSelecting == 2 && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)))
		{
			bool readyToWar = true;
			for (auto& placed : m_isShipPlaced)
			{
				if (!placed)
				{
					readyToWar = false;
					break;
				}
			}
			if (readyToWar)
			{
				m_isWar = true;
				m_playerMap->SetPos(D3DXVECTOR3(-8, 0, 0));
				m_enemyMap->SetPos(D3DXVECTOR3(8, 0, 0));

				m_gameCam->SetObjAt(m_enemyMap->GetPos());
				m_gameCam->SetObjDist(25);
			}
		}

		//배 업뎃
		for (UINT i = 0; i < 5; i++)
		{
			if (m_isShipPlaced[i])
			{
				Ship* ship = m_playerShip[i];
				ship->Update(m_playerMap->PointToVec(ship->GetTilePos()));
			}
		}
	}
	else
	{
		//커서
		if (m_playerMap->GetCursor(m_cursor))
			m_cursorInPlayerMap = true;
		else if (m_enemyMap->GetCursor(m_cursor))
			m_cursorInPlayerMap = false;
		if (SingletonInstance(InputManager)->IsKeyDown(VK_RBUTTON))	m_cursorLookAtY = !m_cursorLookAtY;


		auto CamOnOff = [](const D3DXVECTOR2& pos, const D3DXVECTOR2& size)->bool
		{
			POINT mPos = SingletonInstance(InputManager)->GetMousePos();
			return
				pos.x < mPos.x &&
				pos.y < mPos.y &&
				mPos.x < pos.x + size.x &&
				mPos.y < pos.y + size.y;
		};

		if (SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON) && CamOnOff(D3DXVECTOR2(1200, 30), D3DXVECTOR2(175, 82))) m_gameCam->AutoControl();
		if (SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON) && CamOnOff(D3DXVECTOR2(1200 + 192, 30), D3DXVECTOR2(175, 82))) m_gameCam->PlayerControl();


		//공격
		if (m_isReadyToAttack)
		{
			bool executeAttack = false;
			UINT item = -1;
			POINT attackPos;	attackPos.x = attackPos.y = -1;
			bool isLookAtY;

			if (m_isPlayerTurn)
			{
				auto ItemSelecting = [](const D3DXVECTOR2& pos, const D3DXVECTOR2& size)->bool
				{
					POINT mPos = SingletonInstance(InputManager)->GetMousePos();
					return
						pos.x - size.x * 0.5f < mPos.x &&
						pos.y - size.y * 0.5f < mPos.y &&
						mPos.x < pos.x + size.x * 0.5f &&
						mPos.y < pos.y + size.y * 0.5f;
				};

				m_itemSelecting = -1;
				if (ItemSelecting(D3DXVECTOR2(70 + 62 * 0, 768), D3DXVECTOR2(66, 70))) m_itemSelecting = 0;
				if (ItemSelecting(D3DXVECTOR2(70 + 62 * 1, 768), D3DXVECTOR2(66, 70))) m_itemSelecting = 1;
				if (ItemSelecting(D3DXVECTOR2(70 + 62 * 2, 768), D3DXVECTOR2(66, 70))) m_itemSelecting = 2;
				if (ItemSelecting(D3DXVECTOR2(70 + 62 * 3, 768), D3DXVECTOR2(66, 70))) m_itemSelecting = 3;
				if (SingletonInstance(InputManager)->IsKeyDown('1') || (m_itemSelecting == 0 && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)))	m_selectedItem = m_isPlayerItemUsed[0] ? -1 : 0;
				if (SingletonInstance(InputManager)->IsKeyDown('2') || (m_itemSelecting == 1 && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)))	m_selectedItem = m_isPlayerItemUsed[1] ? -1 : 1;
				if (SingletonInstance(InputManager)->IsKeyDown('3') || (m_itemSelecting == 2 && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)))	m_selectedItem = m_isPlayerItemUsed[2] ? -1 : 2;
				if (SingletonInstance(InputManager)->IsKeyDown('4') || (m_itemSelecting == 3 && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)))	m_selectedItem = m_isPlayerItemUsed[3] ? -1 : 3;


				//스킬 취소 버튼임
				auto CursorInToolButton = [](const D3DXVECTOR2& pos, const float& radius)->bool
				{
					POINT mPos = SingletonInstance(InputManager)->GetMousePos();
					return D3DXVec2Length(&(pos - D3DXVECTOR2(mPos.x, mPos.y))) < radius;
				};
				m_toolSelecting = -1;
				if (CursorInToolButton(D3DXVECTOR2(98 + 66 * 1, 822), 27))	m_toolSelecting = 3;
				if (SingletonInstance(InputManager)->IsKeyDown('R') || (m_toolSelecting == 3 && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)))	m_selectedItem = -1;


				if (m_cursor.x != -1 && !m_cursorInPlayerMap &&
					SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON) &&
					!m_enemyMap->GetTile(m_cursor).second)
				{
					executeAttack = true;
					item = m_selectedItem;
					attackPos = m_cursor;
					isLookAtY = m_cursorLookAtY;

					if (m_selectedItem != -1)
					{
						m_isPlayerItemUsed[m_selectedItem] = true;
						m_selectedItem = -1;
					}
				}
			}
			else
			{
				if (m_aiPlayer->Attack(attackPos))
				{
					UINT leftItem = 0;
					if (!(m_turnCount % 5))
					{
						for (UINT i = 0; i < 4; i++)
						{
							if (!m_isEnemyItemUsed[i])
								leftItem++;
						}
					}

					if (leftItem)
					{
						UINT randomIndex = rand() % leftItem;
						UINT index = 0;
						do
						{
							if (m_isEnemyItemUsed[index])
								index++;
							else if (randomIndex)
							{
								randomIndex--;
								index++;
							}
							else
								break;
						} while (true);

						item = index;
						if (item == 0)
							isLookAtY = rand() & 1;
						
						m_isEnemyItemUsed[item] = true;
					}
					else
 						item = -1;

					executeAttack = true;
				}
			}

			if (executeAttack)
			{
				Ship* launcher = GetRandomLiveShip(m_isPlayerTurn);
				if (m_isPlayerTurn)
				{
					m_gameCam->SetObjAt(launcher->GetPos());
					m_gameCam->SetObjDist(15);
				}

				if (m_isPlayerTurn && SingletonInstance(InputManager)->IsKeyPressed('R'))
					JUSTKILL(launcher, true);
				else
				{
					switch (item)
					{
					case -1:	DefaultAttack	(launcher, attackPos, m_isPlayerTurn);	break;
					case 0:		LineAttack		(launcher, isLookAtY ? attackPos.x : attackPos.y, isLookAtY, m_isPlayerTurn);	break;
					case 1:		RandomAttack	(launcher, m_isPlayerTurn);			break;
					case 2:		_33Attack		(launcher, attackPos, m_isPlayerTurn);	break;
					case 3:		Scanning		(launcher, attackPos, m_isPlayerTurn);	break;
					}
				}

				m_isReadyToAttack = false;
				///m_isReadyToAttack = true;
				///m_isPlayerTurn = false;
			}
		}


		//Launcher
		{
			for (auto& lockonerIter = m_listLockoner.begin(); lockonerIter != m_listLockoner.end(); NULL)
			{
				auto& lockoner = *lockonerIter;

				lockoner->Update();
				if (lockoner->IsReadyToFire())
				{
					Ship* launcher = lockoner->GetLauncher();
					Missile* missile = new Missile(lockoner->GetLockonPos(), lockoner->ByPlayer());

					D3DXVECTOR3 pos = launcher->GetPos();
					D3DXVECTOR3 dir; {
						D3DXMATRIX rm;
						D3DXMatrixRotationY(&rm, D3DXToRadian(rand() % 3600 / 10.));
						D3DXVec3TransformNormal(&dir, &D3DXVECTOR3(0, 0, 1), &rm);
					}
					D3DXVECTOR3 terminal(pos + D3DXVECTOR3(0, 50, 0));

					missile->SetState(pos, dir, terminal, false);
					m_listMissile.push_back(missile);

					SAFE_DELETE(lockoner);
					lockonerIter = m_listLockoner.erase(lockonerIter);
				}
				else
					lockonerIter++;
			}

			for (auto& missileIter = m_listMissile.begin(); missileIter != m_listMissile.end(); NULL)
			{
				auto& missile = *missileIter;

				missile->Update();
				if (missile->IsDiving() || missile->ByPlayer())
					m_listEffect.push_back(new MissileTail(missile->GetPos()));

				Map* targetMap = missile->ByPlayer() ? m_enemyMap : m_playerMap;
				if (!missile->IsDiving())
				{
					if (missile->GetHeight() > 50)
					{
						D3DXVECTOR3 terminal = targetMap->PointToVec( missile->GetLockonPos() );
						D3DXVECTOR3 dir; {
							D3DXMATRIX rm;
							D3DXMatrixRotationY(&rm, D3DXToRadian(rand() % 3600 / 10.));
							D3DXVec3TransformNormal(&dir, &D3DXVECTOR3(0, 0, 1), &rm);
						}
						D3DXVECTOR3 pos = (terminal + D3DXVECTOR3(0, 50, 0)  + dir * 10);

						missile->SetState(pos, dir, terminal, true);


						m_gameCam->SetObjAt(targetMap->GetPos());
						m_gameCam->SetObjDist(25);
					}
					missileIter++;
				}
				else
				{
					if (missile->GetHeight() < 0)
					{
						auto& lockonPos = missile->GetLockonPos();
						auto& tile = targetMap->GetTile(lockonPos);
						
						if (!tile.second && tile.first)
							tile.first->Hit();

						tile.second = true;
						if (!missile->ByPlayer())
						{
							if (tile.first)
								m_aiPlayer->PutHitData(lockonPos);
							m_aiPlayer->PutAttackData(lockonPos);
						}
						m_listEffect.push_back(new MissileAttackEffect(targetMap->PointToVec(lockonPos)));
						m_gameCam->SetCamShake(20);

						SAFE_DELETE(missile);
						missileIter = m_listMissile.erase(missileIter);
					}
					else
						missileIter++;
				}
			}

			if (m_scanner)
			{
				m_scanner->Update();
				if (m_scanner->Complate() && !m_scanner->IsUsed())
				{
					Map* targetMap = m_scanner->ByPlayer() ? m_enemyMap : m_playerMap;

					for (int y = -1; y < 2; y++)
					{
						for (int x = -1; x < 2; x++)
						{
							POINT scanPos = m_scanner->GetScanningPos();
							scanPos.x += x;
							scanPos.y += y;

							if (scanPos.x < 0 ||
								scanPos.x > 9 ||
								scanPos.y < 0 ||
								scanPos.y > 9 ||
								targetMap->GetTile(scanPos).first)
							{
								//
								m_listEffect.push_back(new LockonEffect(targetMap->PointToVec(scanPos)));

								if (!m_scanner->ByPlayer())
									m_aiPlayer->PutScanData(scanPos);
							}
						}
					}
					m_scanner->SetUsed();
				}
				if (m_scanner->IsEnd())
					SAFE_DELETE( m_scanner );
			}
		}


		if (!m_win &&
			!m_isReadyToAttack &&
			m_listLockoner.empty() && m_listMissile.empty() && !m_scanner)
		{
			m_isReadyToAttack = true;
			m_isPlayerTurn = !m_isPlayerTurn;
			if (m_isPlayerTurn)
				m_turnCount++;

			m_gameCam->SetObjAt((m_isPlayerTurn ? m_enemyMap : m_playerMap)->GetPos());
			m_gameCam->SetObjDist(25);
		}



		//배 업뎃
		for (auto& ship : m_playerShip)
		{
			ship->Update(m_playerMap->PointToVec(ship->GetTilePos()));

			if (!ship->GetLife())
			{
				D3DXVECTOR3 pos = m_playerMap->PointToVec(ship->GetTilePos());
				for (UINT i = 0; i < ship->GetSize(); i++)
				{
					m_listEffect.push_back(new ShipSmoke(pos));
					(ship->IsLookAtY() ? pos.z : pos.x) += 1;
				}
			}
		}
		for (auto& ship : m_enemyShip)
		{
			ship->Update(m_enemyMap->PointToVec(ship->GetTilePos()));

			if (!ship->GetLife())
			{
				D3DXVECTOR3 pos = m_enemyMap->PointToVec(ship->GetTilePos());
				for (UINT i = 0; i < ship->GetSize(); i++)
				{
					m_listEffect.push_back(new ShipSmoke(pos));
					(ship->IsLookAtY() ? pos.z : pos.x) += 1;
				}
			}
		}

		m_playerUI->Update();
		m_enemyUI->Update();

		{
			bool eWin = true, pWin = true;
			for (auto& ship : m_playerShip)
				if (ship->GetLife())
					eWin = false;
			for (auto& ship : m_enemyShip)
				if (ship->GetLife())
					pWin = false;

			if (pWin)
			{
				m_win = true;
				m_pWin = true;
				m_isReadyToAttack = false;
			}
			if (eWin)
			{
				m_win = true;
				m_pWin = false;
				m_isReadyToAttack = false;
			}
		}

	}

	for (auto& effectIter = m_listEffect.begin(); effectIter != m_listEffect.end(); NULL)
	{
		Effect* effect = *effectIter;
		effect->Update();

		if (effect->IsExpired())
		{
			SAFE_DELETE( effect );
			effectIter = m_listEffect.erase(effectIter);
		}
		else
			effectIter++;
	}

	m_gameCam->Update();
	m_gameCam->Render();
}

void InGame::Render()
{
	SingletonInstance(SkySphere)->Render();
	
	if (!m_isWar)
	{
		//배
		for (UINT i = 0; i < 5; i++)
		{
			if (m_isShipPlaced[i])
				m_playerShip[i]->Render();
		}

		m_playerMap->RenderMap(false, TextureGet(L"pMapFrame"));

		//커서
		if (m_cursor.x != -1)
		{
			if (m_selectedShip == -1)
				m_playerMap->RenderTile(m_cursor, TextureGet(L"TileS"));
			else
			{
				UINT size = m_playerShip[m_selectedShip]->GetSize();
				POINT checkPos = m_cursor;
				auto& upper = (m_cursorLookAtY ? checkPos.y : checkPos.x);
				for (UINT i = 0; i < size; i++)
				{
					m_playerMap->RenderTile(checkPos, TextureGet(m_isCanPlace ? L"TileT" : L"TileF"));
					upper++;
				}
			}
		}


		SPRITE->Begin(D3DXSPRITE_ALPHABLEND);
		
		m_playerUI->R_Frame(D3DXVECTOR2(15, 145));

		if (!m_isShipPlaced[0])	m_playerUI->R_Ship(D3DXVECTOR2(41, 255),	2, m_selectedShip == 0, false);
		if (!m_isShipPlaced[1])	m_playerUI->R_Ship(D3DXVECTOR2(179, 255),	2, m_selectedShip == 1, false);
		if (!m_isShipPlaced[2])	m_playerUI->R_Ship(D3DXVECTOR2(41, 380),	3, m_selectedShip == 2, false);
		if (!m_isShipPlaced[3])	m_playerUI->R_Ship(D3DXVECTOR2(41, 505),	3, m_selectedShip == 3, false);
		if (!m_isShipPlaced[4])	m_playerUI->R_Ship(D3DXVECTOR2(41, 630),	4, m_selectedShip == 4, false);

		m_playerUI->R_Item(D3DXVECTOR2(70 + 62 * 0, 790), 0, false);
		m_playerUI->R_Item(D3DXVECTOR2(70 + 62 * 1, 790), 1, false);
		m_playerUI->R_Item(D3DXVECTOR2(70 + 62 * 2, 790), 2, false);
		m_playerUI->R_Item(D3DXVECTOR2(70 + 62 * 3, 790), 3, false);

		m_playerUI->R_Tool(D3DXVECTOR2(98 + 66 * 0, 850), 0, m_toolSelecting == 0);
		m_playerUI->R_Tool(D3DXVECTOR2(98 + 66 * 1, 850), 1, m_toolSelecting == 1);
		m_playerUI->R_Tool(D3DXVECTOR2(98 + 66 * 2, 850), 2, m_toolSelecting == 2);
		
		if (m_toolSelecting != -1)
			m_playerUI->R_ToolBox(m_toolSelecting);

		SPRITE->End();
	}
	else
	{
		//배
		for (auto& ship : m_playerShip)	ship->Render();
		for (auto& ship : m_enemyShip)	if (!ship->GetLife())	ship->Render();

		//미사일
		for (auto& missile : m_listMissile)
		{
			if (missile->IsDiving() || missile->ByPlayer())
				missile->Render();
		}


		m_playerMap->RenderMap(!m_isPlayerTurn, TextureGet(L"pMapFrame"));
		m_enemyMap->RenderMap(m_isPlayerTurn, TextureGet(L"eMapFrame"));

		//커서
		if (m_cursor.x != -1)
		{
			if (m_cursorInPlayerMap)
				m_playerMap->RenderTile(m_cursor, TextureGet(L"TileS"));
			else
			{
				switch (m_selectedItem)
				{
				case -1:
				case 1:
					m_enemyMap->RenderTile(m_cursor, TextureGet(L"TileS"));
					break;

				case 0:
				{
					POINT pos = m_cursor;
					auto& upper = m_cursorLookAtY ? pos.y : pos.x;
					for (UINT i = 0; i < 10; i++)
					{
						upper = i;
						m_enemyMap->RenderTile(pos, TextureGet(L"TileS"));
					}
				}
					break;

				case 2:
				case 3:
					for (int x = -1; x < 2; x++)
					{
						for (int y = -1; y < 2; y++)
						{
							POINT pos = m_cursor;
							pos.x += x;
							pos.y += y;

							if (pos.x < 0 ||
								pos.y < 0 ||
								pos.x > 9 ||
								pos.y > 9)
								continue;

							m_enemyMap->RenderTile(pos, TextureGet(L"TileS"));
						}
					}
					break;
				}
			}
		}

		if (m_scanner)
		{
			Map* targetMap = m_scanner->ByPlayer() ? m_enemyMap : m_playerMap;
			m_scanner->Render(targetMap->PointToVec(m_scanner->GetScanningPos()));
		}

		ZBufOff();
		ZBufWriteOff();

		for (auto& effect : m_listEffect)
			effect->Render();

		for (auto& lockoner : m_listLockoner)
		{
			auto& map = lockoner->ByPlayer() ? m_enemyMap : m_playerMap;
			lockoner->Render(map->PointToVec(lockoner->GetLockonPos()));
		}

		ZBufOn();
		ZBufWriteOn();
		


		SPRITE->Begin(D3DXSPRITE_ALPHABLEND);

		m_enemyUI->R_Frame(D3DXVECTOR2(1600 - 300 - 15, 145));

		m_enemyUI->R_Ship(D3DXVECTOR2(1600 - 123 - 41, 255), 2, true, !m_enemyShip[0]->GetLife());
		m_enemyUI->R_Ship(D3DXVECTOR2(1600 - 123 - 179, 255), 2, true, !m_enemyShip[1]->GetLife());
		m_enemyUI->R_Ship(D3DXVECTOR2(1600 - 188 - 41, 380), 3, true, !m_enemyShip[2]->GetLife());
		m_enemyUI->R_Ship(D3DXVECTOR2(1600 - 188 - 41, 505), 3, true, !m_enemyShip[3]->GetLife());
		m_enemyUI->R_Ship(D3DXVECTOR2(1600 - 257 - 41, 630), 4, true, !m_enemyShip[4]->GetLife());

		m_enemyUI->R_Item(D3DXVECTOR2(1600 - 70 - (62 * 3), 790), 0, !m_isEnemyItemUsed[0]);
		m_enemyUI->R_Item(D3DXVECTOR2(1600 - 70 - (62 * 2), 790), 1, !m_isEnemyItemUsed[1]);
		m_enemyUI->R_Item(D3DXVECTOR2(1600 - 70 - (62 * 1), 790), 2, !m_isEnemyItemUsed[2]);
		m_enemyUI->R_Item(D3DXVECTOR2(1600 - 70 - (62 * 0), 790), 3, !m_isEnemyItemUsed[3]);



		m_playerUI->R_Frame(D3DXVECTOR2(15, 145));

		m_playerUI->R_Ship(D3DXVECTOR2(41, 255),	2, true, !m_playerShip[0]->GetLife());
		m_playerUI->R_Ship(D3DXVECTOR2(179, 255),	2, true, !m_playerShip[1]->GetLife());
		m_playerUI->R_Ship(D3DXVECTOR2(41, 380),	3, true, !m_playerShip[2]->GetLife());
		m_playerUI->R_Ship(D3DXVECTOR2(41, 505),	3, true, !m_playerShip[3]->GetLife());
		m_playerUI->R_Ship(D3DXVECTOR2(41, 630),	4, true, !m_playerShip[4]->GetLife());

		m_playerUI->R_Item(D3DXVECTOR2(70 + 62 * 0, 790), 0, m_itemSelecting == 0 || m_selectedItem == 0);
		m_playerUI->R_Item(D3DXVECTOR2(70 + 62 * 1, 790), 1, m_itemSelecting == 1 || m_selectedItem == 1);
		m_playerUI->R_Item(D3DXVECTOR2(70 + 62 * 2, 790), 2, m_itemSelecting == 2 || m_selectedItem == 2);
		m_playerUI->R_Item(D3DXVECTOR2(70 + 62 * 3, 790), 3, m_itemSelecting == 3 || m_selectedItem == 3);

		m_playerUI->R_Tool(D3DXVECTOR2(98 + 66 * 1, 850), 1, m_toolSelecting == 3);

		if (m_toolSelecting != -1)
			m_playerUI->R_ToolBox(m_toolSelecting);

		D3DXMATRIX iden;
		D3DXMatrixIdentity(&iden);
		SPRITE->SetTransform(&iden);
		auto SetTM = [](const D3DXVECTOR2& pos)
		{
			D3DXMATRIX tm;
			D3DXMatrixTranslation(&tm, pos.x, pos.y, 0);
			SPRITE->SetTransform(&tm);
		};

		SetTM(D3DXVECTOR2(1168, 0));	SPRITE->Draw(TextureGet(L"CamFrame")->tex,	nullptr,nullptr,nullptr, D3DXCOLOR(1, 1, 1, 1));
		SetTM(D3DXVECTOR2(1200, 30));	SPRITE->Draw(TextureGet(L"CamA")->tex,	nullptr,nullptr,nullptr, m_gameCam->IsPlayerControl() ? D3DXCOLOR(0.5, 0.5, 0.5, 1) : D3DXCOLOR(1, 1, 1, 1));
		SetTM(D3DXVECTOR2(1392, 30));	SPRITE->Draw(TextureGet(L"CamP")->tex,	nullptr,nullptr,nullptr, !m_gameCam->IsPlayerControl() ? D3DXCOLOR(0.5, 0.5, 0.5, 1) : D3DXCOLOR(1, 1, 1, 1));
		SetTM(D3DXVECTOR2(1185, 137));	SPRITE->Draw(TextureGet(m_gameCam->IsPlayerControl() ? L"CamBoxP" : L"CamBoxA")->tex,	nullptr,nullptr,nullptr, D3DXCOLOR(1, 1, 1, 1));

		SetTM(D3DXVECTOR2(0, 0));	SPRITE->Draw(TextureGet(L"ScoreFrame")->tex, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, 1));

		SetTM(D3DXVECTOR2((1600 - 471) * 0.5f, 0));	SPRITE->Draw(TextureGet(m_isPlayerTurn ? L"pTurn" : L"eTurn")->tex, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, 1));

		if (m_win)
		{
			float scale = 10.f - m_winFrame;
			if (scale < 1)
				scale = 1;

			D3DXMATRIX sm, tm;
			D3DXMatrixScaling(&sm, scale, scale, scale);
			D3DXMatrixTranslation(&tm, 800, 400, 0);
			SPRITE->SetTransform(&(sm * tm));
			SPRITE->Draw(TextureGet(m_pWin ? L"pWin" : L"eWin")->tex, nullptr, &D3DXVECTOR3(469, 193, 0), nullptr, D3DXCOLOR(1, 1, 1, 1));
		}
		

		SPRITE->End();
	}

}

void InGame::Release()
{
	SAFE_DELETE(m_playerMap);
	SAFE_DELETE(m_enemyMap);
	for (auto ship : m_playerShip)	SAFE_DELETE(ship);	m_playerShip.fill(nullptr);
	for (auto ship : m_enemyShip)	SAFE_DELETE(ship);	m_enemyShip.fill(nullptr);
	SAFE_DELETE(m_aiPlayer);
	SAFE_DELETE(m_gameCam);
	

	for (auto lockoner : m_listLockoner)	SAFE_DELETE(lockoner);	m_listLockoner.clear();
	for (auto missile : m_listMissile)		SAFE_DELETE(missile);	m_listMissile.clear();
	SAFE_DELETE(m_scanner);
	for (auto effect: m_listEffect)			SAFE_DELETE(effect);	m_listEffect.clear();

	SAFE_DELETE(m_playerUI);
	SAFE_DELETE(m_enemyUI);
}




bool InGame::CanPlaceShip(Ship * target, const POINT & pos, const bool & isLookAtY, const bool & byPlayer)
{
	Map* targetMap = (byPlayer ? m_playerMap : m_enemyMap);
	UINT size = target->GetSize();

	POINT checkPos = pos;
	auto& upper = (isLookAtY ? checkPos.y : checkPos.x);
	for (UINT i = 0; i < size; i++)
	{
		if (checkPos.x < 0 ||
			checkPos.y < 0 ||
			checkPos.x > 9 ||
			checkPos.y > 9 ||
			targetMap->GetTile(checkPos).first)
			return false;
		upper++;
	}
	return true;
}

void InGame::PlaceShip(Ship * target, const POINT & pos, const bool & isLookAtY, const bool & byPlayer)
{
	Map* targetMap = (byPlayer ? m_playerMap : m_enemyMap);
	UINT size = target->GetSize();

	POINT checkPos = pos;
	auto& upper = (isLookAtY ? checkPos.y : checkPos.x);
	for (UINT i = 0; i < size; i++)
	{
		targetMap->GetTile(checkPos).first = target;
		upper++;
	}

	target->SetPlaced(pos, isLookAtY);
}

void InGame::RandomPlaceShip(const bool & byPlayer)
{
	auto& shipList = byPlayer ? m_playerShip : m_enemyShip;
	for (UINT i = 0; i < 5; NULL)
	{
		if (byPlayer && m_isShipPlaced[i])
		{
			i++;
			continue;
		}

		Ship* ship = shipList[i];

		POINT pos;
		pos.x = rand() % 10;
		pos.y = rand() % 10;
		bool isLookAtY = rand() & 1;

		if (CanPlaceShip(ship, pos, isLookAtY, byPlayer))
		{
			PlaceShip(ship, pos, isLookAtY, byPlayer);
			i++;
		}
	}
	if (byPlayer)
		m_isShipPlaced.fill(true);
}

void InGame::ResetPlace(const bool & byPlayer)
{
	(byPlayer ? m_playerMap : m_enemyMap)->MapClear();
	if (byPlayer)
		m_isShipPlaced.fill(false);
}



Ship * InGame::GetRandomLiveShip(const bool & byPlayer)
{
	auto& shipList = byPlayer ? m_playerShip : m_enemyShip;

	UINT liveCount = 0;
	for (auto& ship : shipList)
	{
		if (ship->GetLife())
			liveCount++;
	}

	UINT randomIndex = rand() % liveCount;
	UINT index = 0;
	do
	{
		if (!shipList[index]->GetLife())
			index++;
		else if (randomIndex)
		{
			randomIndex--;
			index++;
		}
		else
			break;
	} while (true);

	return shipList[index];
}



void InGame::DefaultAttack(Ship * launcher, const POINT & attackPos, const bool & byPlayer)
{
	for (UINT i = 0; i < 8; i++)
		m_listLockoner.push_back(new Lockoner(launcher, attackPos, byPlayer, i * 5, i == 0));
}

void InGame::LineAttack(Ship * launcher, const UINT & line, const bool & isLookAtY, const bool & byPlayer)
{
	POINT pos;
	(isLookAtY ? pos.x : pos.y) = line;
	auto& upper = isLookAtY ? pos.y : pos.x;
	for (UINT i = 0; i < 10; i++)
	{
		upper = i;
		m_listLockoner.push_back(new Lockoner(launcher, pos, byPlayer, i * 5));
	}
}

void InGame::RandomAttack(Ship * launcher, const bool & byPlayer)
{
	Map* targetMap = (byPlayer ? m_enemyMap : m_playerMap);
	
	UINT canPlaceCount = 0;
	for (UINT i = 0; i < 100; i++)
	{
		if (!targetMap->GetTile(i).second)
			canPlaceCount++;
	}
	
	if (!canPlaceCount)
		return;
	else if (canPlaceCount > 8)
		canPlaceCount = 8;

	std::array<UINT, 8> prevInput;
	prevInput.fill(-1);
	for (UINT i = 0; i < canPlaceCount; NULL)
	{
		UINT pos = rand() % 100;

		bool isSame = false;
		for (UINT j = 0; j < i; j++)
		{
			if (prevInput[j] == pos)
			{
				isSame = true;
				break;
			}
		}
		if (isSame || targetMap->GetTile(pos).second)
			continue;

		prevInput[i] = pos;
		m_listLockoner.push_back(new Lockoner(launcher, Map::LineToPoint(pos), byPlayer, i * 5));
		i++;
	}
}

void InGame::_33Attack(Ship * launcher, const POINT & attackPos, const bool & byPlayer)
{
	for (int x = -1; x < 2; x++)
	{
		for (int y = -1; y < 2; y++)
		{
			POINT pos = attackPos;
			pos.x += x;
			pos.y += y;

			if (pos.x < 0 ||
				pos.y < 0 ||
				pos.x > 9 ||
				pos.y > 9)
				continue;

			m_listLockoner.push_back(new Lockoner(launcher, pos, byPlayer, ((x+1) + (y+1) * 3) * 5));
		}
	}
}

void InGame::Scanning(Ship * launcher, const POINT & attackPos, const bool & byPlayer)
{
	m_scanner = new Scanner(attackPos, byPlayer);

	m_gameCam->SetObjAt((byPlayer ? m_enemyMap : m_playerMap)->PointToVec(attackPos));
	m_gameCam->SetObjDist(15);
}

void InGame::JUSTKILL(Ship * launcher, const bool & byPlayer)
{
	Map* targetMap = (byPlayer ? m_enemyMap : m_playerMap);

	UINT canPlaceCount = 0;
	for (UINT i = 0; i < 100; i++)
	{
		if (!targetMap->GetTile(i).second)
			canPlaceCount++;
	}

	if (!canPlaceCount)
		return;

	std::array<UINT, 100> prevInput;
	prevInput.fill(-1);
	for (UINT i = 0; i < canPlaceCount; NULL)
	{
		UINT pos = rand() % 100;

		bool isSame = false;
		for (UINT j = 0; j < i; j++)
		{
			if (prevInput[j] == pos)
			{
				isSame = true;
				break;
			}
		}
		if (isSame || targetMap->GetTile(pos).second)
			continue;

		prevInput[i] = pos;
		m_listLockoner.push_back(new Lockoner(launcher, Map::LineToPoint(pos), byPlayer, i * 5));
		i++;
	}
}

