#include "DXUT.h"
#include "InGame.h"

#include "Map.h"
#include "Ship.h"

#include "Lockon.h"
#include "Missile.h"
#include "Scanner.h"

#include "Effect.h"
#include "AttackHit.h"
#include "AttackMiss.h"
#include "MissileTail.h"
#include "ScanEffect.h"

#include "GameAI.h"


InGame::InGame()
	: m_playerMap(nullptr)
	, m_enemyMap(nullptr)
{
	m_playerShip.fill(nullptr);
	m_enemyShip.fill(nullptr);
}
InGame::~InGame()
{
}



void InGame::Initialize()
{
	TextureLoad(L"./Resource/map/Tile_0.png", L"Tile");
	TextureLoad(L"./Resource/map/Tile_1.png", L"TileF");
	TextureLoad(L"./Resource/map/Tile_2.png", L"TileT");
	TextureLoad(L"./Resource/map/Tile_Selected.png", L"TileS");

	/* Load Ship */ {
		ASELoad(L"./Resource/model", L"T2.ASE",			L"ShipX2");
		ASELoad(L"./Resource/model", L"T2_Broken.ASE",	L"ShipX2D");

		ASELoad(L"./Resource/model", L"T3.ASE",			L"ShipX3");
		ASELoad(L"./Resource/model", L"T3_2.ASE",		L"ShipX3Wing");
		ASELoad(L"./Resource/model", L"T3_Broken.ASE",	L"ShipX3D");

		ASELoad(L"./Resource/model", L"T4.ASE",			L"ShipX4");
		ASELoad(L"./Resource/model", L"T4_Broken.ASE",	L"ShipX4D");
	}

	/* Launch */ {
		for (UINT i = 0; i < 10; i++)
			TextureLoad(L"./Resource/map/Click_" + std::to_wstring(i) + L".png", L"Lockoner" + std::to_wstring(i));

		ASELoad(L"./Resource/model", L"Missile.ASE", L"Missile");

		TextureLoad(L"./Resource/map/Scanner0.png", L"Scanner0");
		TextureLoad(L"./Resource/map/Scanner1.png", L"Scanner1");
	}

	/* Effect */ {
		for (UINT i = 1; i < 28; i++)
			TextureLoad(L"./Resource/effect/attackHit/" + std::to_wstring(i) + L".png", L"AttackHit" + std::to_wstring(i - 1));
		for (UINT i = 1; i < 25; i++)
			TextureLoad(L"./Resource/effect/attackMiss/" + std::to_wstring(i) + L".png", L"AttackMiss" + std::to_wstring(i - 1));
	}




	m_isWar = false;

	/* Player */ {
		m_playerMap = new Map();
		m_playerMap->ClearMap();
		m_playerShip[0] = new Ship(2);
		m_playerShip[1] = new Ship(2);
		m_playerShip[2] = new Ship(3);
		m_playerShip[3] = new Ship(3);
		m_playerShip[4] = new Ship(4);

		m_enemyMap = new Map();
		m_enemyMap->ClearMap();
		m_enemyShip[0] = new Ship(2);
		m_enemyShip[1] = new Ship(2);
		m_enemyShip[2] = new Ship(3);
		m_enemyShip[3] = new Ship(3);
		m_enemyShip[4] = new Ship(4);

		ShipRandomPlace(false);
		m_aiPlayer = new GameAI();
	}


	m_cursor.x = m_cursor.y = -1;
	m_cursorInPlayerMap = true;
	m_cursorLookAtY = false;

	m_selectedShip = -1;
	m_isShipPlaced.fill(false);
	m_isShipCanPlace = false;

	m_isReadyToAttack = true;
	m_isPlayerTurn = true;
	m_isItemUsed.fill(false);
	m_selectedItem = -1;
	m_turnCount = 1;

	m_scanner = nullptr;
}

void InGame::Update()
{
	SingletonInstance(SkySphere)->Update();


	//커서 업뎃
	{
		if (m_playerMap->GetMousePos(m_cursor))
			m_cursorInPlayerMap = true;
		else if (m_enemyMap->GetMousePos(m_cursor))
			m_cursorInPlayerMap = false;

		if (SingletonInstance(InputManager)->IsKeyDown(VK_RBUTTON))
			m_cursorLookAtY = !m_cursorLookAtY;
	}
	
	//배 업데이트
	auto ShipUpdate = [](Ship* ship, Map* map)
	{
		ship->Update(map->PointToVec(ship->GetTilePos()));
	};


	if (!m_isWar)
	{
		// 설치할 배 선택
		if (SingletonInstance(InputManager)->IsKeyDown('1'))	m_selectedShip = (m_isShipPlaced[0] ? -1 : 0);
		if (SingletonInstance(InputManager)->IsKeyDown('2'))	m_selectedShip = (m_isShipPlaced[1] ? -1 : 1);
		if (SingletonInstance(InputManager)->IsKeyDown('3'))	m_selectedShip = (m_isShipPlaced[2] ? -1 : 2);
		if (SingletonInstance(InputManager)->IsKeyDown('4'))	m_selectedShip = (m_isShipPlaced[3] ? -1 : 3);
		if (SingletonInstance(InputManager)->IsKeyDown('5'))	m_selectedShip = (m_isShipPlaced[4] ? -1 : 4);

		//커서 보정
		if (m_selectedShip != -1)
		{
			UINT shipSize = m_playerShip[m_selectedShip]->GetSize();

			auto& target = (m_cursorLookAtY ? m_cursor.y : m_cursor.x);
			if (target > 10 - shipSize)
				target = 10 - shipSize;
		}

		//배 설치
		if (m_selectedShip != -1 && m_cursorInPlayerMap)
		{
			m_isShipCanPlace = IsShipCanPlace(m_playerShip[m_selectedShip], m_cursor, m_cursorLookAtY, true);

			if (SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON) &&
				m_isShipCanPlace)
			{
				ShipPlace(m_playerShip[m_selectedShip], m_cursor, m_cursorLookAtY, true);
				m_isShipPlaced[m_selectedShip] = true;
				m_selectedShip = -1;
			}
		}

		//배 초기화
		if (SingletonInstance(InputManager)->IsKeyDown('R'))
		{
			m_playerMap->ClearMap();
			m_isShipPlaced.fill(false);
			m_selectedShip = -1;
		}

		//배 자동배치
		if (SingletonInstance(InputManager)->IsKeyDown('A'))
			ShipRandomPlace(true);
		
		//게임 시작
		if (SingletonInstance(InputManager)->IsKeyDown('S'))
		{
			bool placeSucceeded = true;
			for (auto& isShipPlace : m_isShipPlaced)
			{
				if (!isShipPlace)
					placeSucceeded = false;
			}

			if (placeSucceeded)
			{
				m_playerMap->SetPos(D3DXVECTOR3(-6, 0, 0));
				m_enemyMap->SetPos(D3DXVECTOR3(+6, 0, 0));

				m_isWar = true;
			}
		}



		// 배 업뎃
		for (UINT i = 0; i < 5; i++)
		{
			if (m_isShipPlaced[i])
				ShipUpdate(m_playerShip[i], m_playerMap);
		}
	}
	else
	{
		//Ship
		for (auto& ship : m_playerShip)	ShipUpdate(ship, m_playerMap);
		for (auto& ship : m_enemyShip)	ShipUpdate(ship, m_enemyMap);


		if (m_isReadyToAttack)
		{
			bool executeAttack = false;
			UINT weapon = -1;
			POINT attackPos; attackPos.x = attackPos.y = -1;
			bool isLookAtY = false;

			if (m_isPlayerTurn)
			{
				//아이템 초기화
				if (SingletonInstance(InputManager)->IsKeyDown(VK_NUMPAD1))	m_isItemUsed.fill(false);


				// 아이템 선택
				if (SingletonInstance(InputManager)->IsKeyDown('1'))		m_selectedItem = m_isItemUsed[0] ? -1 : 0;
				if (SingletonInstance(InputManager)->IsKeyDown('2'))		m_selectedItem = m_isItemUsed[1] ? -1 : 1;
				if (SingletonInstance(InputManager)->IsKeyDown('3'))		m_selectedItem = m_isItemUsed[2] ? -1 : 2;
				if (SingletonInstance(InputManager)->IsKeyDown('4'))		m_selectedItem = m_isItemUsed[3] ? -1 : 3;
				if (SingletonInstance(InputManager)->IsKeyDown(VK_OEM_3))	m_selectedItem = -1;

				if (m_cursor.x != -1 && !m_cursorInPlayerMap &&
					SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON) &&
					(m_selectedItem != -1 || !m_enemyMap->GetTile(m_cursor).second))
				{
					executeAttack = true;
					weapon = m_selectedItem;
					attackPos = m_cursor;
					isLookAtY = m_cursorLookAtY;
				}
			}
			else
			{
				if (m_turnCount && m_turnCount % 5 == 0 &&
					m_aiPlayer->DeclarationUseItem(weapon))
				{
					switch (weapon)
					{
					case 0:	
					{
						UINT line = -1;
						executeAttack = m_aiPlayer->LineAttack(line, isLookAtY);
						(isLookAtY ? attackPos.x : attackPos.y) = line;
						break;
					}
					case 1:	executeAttack = true;	break;
					case 2:	executeAttack = m_aiPlayer->_33Attack(attackPos);	break;
					case 3:	executeAttack = m_aiPlayer->Scanning(attackPos);	break;
					}
				}
				else
				{
					weapon = -1;
					executeAttack = m_aiPlayer->Attack(attackPos);
				}
			}

			if (executeAttack)
			{
				switch (weapon)
				{
				case -1:	DefaultAttack	(GetRandomLiveShip(m_isPlayerTurn),	attackPos, m_isPlayerTurn);			break;
				case 0:		LineAttack		(GetRandomLiveShip(m_isPlayerTurn), isLookAtY ? attackPos.x : attackPos.y, isLookAtY, m_isPlayerTurn);	break;
				case 1:		RandomAttack	(GetRandomLiveShip(m_isPlayerTurn),	m_isPlayerTurn);					break;
				case 2:		_33Attack		(GetRandomLiveShip(m_isPlayerTurn),	attackPos, m_isPlayerTurn);			break;
				case 3:		Scanning		(GetRandomLiveShip(m_isPlayerTurn),	attackPos, m_isPlayerTurn);			break;
				}

				m_isReadyToAttack = false;
				if (m_isPlayerTurn)
				{
					if (m_selectedItem != -1)
						m_isItemUsed[m_selectedItem] = true;
					m_selectedItem = -1;
				}
			}
		}
		

		//Launcher
		{
			for (auto& lockonIter = m_listLockon.begin(); lockonIter != m_listLockon.end(); NULL)
			{
				auto& lockon = *lockonIter;

				lockon->Update();
				if (lockon->LockonComplate())
				{
					D3DXVECTOR3 missilePos(lockon->GetLauncher()->GetPos());
					D3DXVECTOR3 missileDir; {
						D3DXMATRIX rm;
						D3DXMatrixRotationY(&rm, RADIAN(rand() % 3600 / 10.));
						D3DXVec3TransformNormal(&missileDir, &D3DXVECTOR3(0, 0, 1), &rm);
					}
					D3DXVECTOR3 missileTerminal(D3DXVECTOR3(0, 50, 0) + missilePos);

					Missile* missile = new Missile(lockon->GetLockonPos(), lockon->ByPlayer());
					missile->Resetting(missilePos, missileDir, missileTerminal, false);
					m_listMissile.push_back(missile);


					SAFE_DELETE(lockon);
					lockonIter = m_listLockon.erase(lockonIter);
				}
				else
					lockonIter++;
			}

			for (auto& missileIter = m_listMissile.begin(); missileIter != m_listMissile.end(); NULL)
			{
				auto& missile = *missileIter;

				missile->Update();
				if (!missile->IsDiving())
				{
					if (missile->GetHeight() > 50)
					{
						D3DXVECTOR3 tilePos = (missile->ByPlayer() ? m_enemyMap : m_playerMap)->PointToVec(missile->GetLockonPos());

						D3DXVECTOR3 missilePos(D3DXVECTOR3(0, 50, 0) + tilePos);
						D3DXVECTOR3 missileDir; {
							D3DXMATRIX rm;
							D3DXMatrixRotationY(&rm, RADIAN(rand() % 3600 / 10.));
							D3DXVec3TransformNormal(&missileDir, &D3DXVECTOR3(0, 0, 1), &rm);
						}
						missilePos += missileDir * 10;
						D3DXVECTOR3 missileTerminal(tilePos);
						missile->Resetting(missilePos, missileDir, missileTerminal, true);


					}
					missileIter++;
				}
				else if (missile->IsDiving())
				{
					if (missile->GetHeight() < 0)
					{
						POINT lockonPos = missile->GetLockonPos();
						m_aiPlayer->PutAttackData(lockonPos);

						auto& tile = (missile->ByPlayer() ? m_enemyMap : m_playerMap)->GetTile(lockonPos);
						tile.second = true;
						if (!missile->ByPlayer() && tile.first)
							m_aiPlayer->PutHitData(lockonPos);

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
				if (m_scanner->IsEnd())
				{
					Map* targetMap = (m_scanner->ByPlayer() ? m_enemyMap : m_playerMap);

					for (int y = -1; y < 2; y++)
					{
						for (int x = -1; x < 2; x++)
						{
							POINT pos = m_scanner->GetLockonPos();
							pos.x += x;
							pos.y += y;
							if (pos.x < 0 ||
								pos.y < 0 ||
								pos.x > 9 ||
								pos.y > 9)
								continue;
							if (targetMap->GetTile(pos).first)
							{
								m_listEffect.push_back(new ScanEffect(targetMap->PointToVec(pos)));
								if (!m_scanner->ByPlayer())
									m_aiPlayer->PutScanData(pos);
							}
						}
					}

					SAFE_DELETE(m_scanner);
				}
			}
		}

		//Effect
		for (auto& effectIter = m_listEffect.begin(); effectIter != m_listEffect.end(); NULL)
		{
			auto& effect = *effectIter;

			effect->Update();
			if (effect->IsExpired())
			{
				SAFE_DELETE(effect);
				effectIter = m_listEffect.erase(effectIter);
			}
			else
				effectIter++;
		}


		//Return
		if (!m_isReadyToAttack &&
			m_listLockon.empty() && m_listMissile.empty() && !m_scanner)
		{
			m_isReadyToAttack = true;
			m_isPlayerTurn = !m_isPlayerTurn;
			if (m_isPlayerTurn)
				m_turnCount++;
		}
	}
}

void InGame::Render()
{
	SingletonInstance(SkySphere)->Render();

	if (!m_isWar)
	{
		//Ship Render
		for (UINT i = 0; i < 5; i++)
		{
			if (m_isShipPlaced[i])
				m_playerShip[i]->Render();
		}

		m_playerMap->RenderMap();
		if (m_cursor.x != -1)
		{
			if (m_selectedShip == -1)
				m_playerMap->RenderTile(m_playerMap->PointToLine(m_cursor), TextureGet(L"TileS"));
			else
			{
				UINT shipSize = m_playerShip[m_selectedShip]->GetSize();

				POINT tilePos = m_cursor;
				auto& upper = (m_cursorLookAtY ? tilePos.y : tilePos.x);
				for (UINT i = 0; i < shipSize; i++)
				{
					m_playerMap->RenderTile(m_playerMap->PointToLine(tilePos), TextureGet(m_isShipCanPlace ? L"TileT" : L"TileF"));
					upper++;
				}
			}
		}
	}
	else
	{
		//Ship
		for (UINT i = 0; i < 5; i++)
		{
			Ship* playerShip = m_playerShip[i];
			Ship* enemyShip = m_enemyShip[i];

			playerShip->Render();
			if (!enemyShip->GetLife())
				enemyShip->Render();
		}

		//Missile
		for (auto& missile : m_listMissile)
		{
			if (missile->IsDiving() || missile->ByPlayer())
				missile->Render();
		}



		//Map
		m_playerMap->RenderMap();
		m_enemyMap->RenderMap();

		//Tile
		{
			auto& cursorMap = m_cursorInPlayerMap ? m_playerMap : m_enemyMap;

			switch (m_selectedItem)
			{
			case -1:
			case 1:
				if (m_cursor.x != -1)
					cursorMap->RenderTile(Map::PointToLine(m_cursor), TextureGet(L"TileS"));
				break;

			case 0:
				if (m_cursor.x != -1)
				{
					POINT pos = m_cursor;
					auto& upper = m_cursorLookAtY ? pos.y : pos.x;
					
					for (UINT i = 0; i < 10; i++)
					{
						upper = i;
						cursorMap->RenderTile(Map::PointToLine(pos), TextureGet(L"TileS"));
					}
				}
				break;
			case 2:
			case 3:
				if (m_cursor.x != -1)
				{
					for (int y = -1; y < 2; y++)
					{
						for (int x = -1; x < 2; x++)
						{
							POINT pos = m_cursor;
							pos.x += x;
							pos.y += y;
							if (pos.x < 0 ||
								pos.y < 0 || 
								pos.x > 9 || 
								pos.y > 9)
								continue;
							cursorMap->RenderTile(Map::PointToLine(pos), TextureGet(L"TileS"));
						}
					}
				}
				break;
			}
		}



		//Scanner
		if (m_scanner)
			m_scanner->Render();

		ZBufferOff();
		ZBufWriteOff();

		//Effect
		for (auto& effect : m_listEffect)
			effect->Render();

		//Lockoner
		for (auto& lockon : m_listLockon)
		{
			D3DXVECTOR3 pos = (lockon->ByPlayer() ? m_enemyMap : m_playerMap)->PointToVec(lockon->GetLockonPos());
			lockon->Render(pos);
		}

		ZBufferOn();
		ZBufWriteOn();
	}
}

void InGame::Release()
{
	SAFE_DELETE(m_aiPlayer);

	SAFE_DELETE(m_playerMap);
	SAFE_DELETE(m_enemyMap);
	for (UINT i = 0; i < 5; i++)
	{
		SAFE_DELETE(m_playerShip[i]);
		SAFE_DELETE(m_enemyShip[i]);
	}

	for (auto& lockon : m_listLockon)	SAFE_DELETE( lockon );
	for (auto& missile : m_listMissile)	SAFE_DELETE(missile);
	SAFE_DELETE(m_scanner);
	for (auto& effect : m_listEffect)	SAFE_DELETE(effect);
}



bool InGame::IsShipCanPlace(Ship* target, const POINT & pos, const bool & isLookAtY, const bool & byPlayer)
{
	UINT shipSize = target->GetSize();
	Map* targetMap = byPlayer ? m_playerMap : m_enemyMap;

	if (pos.x < 0 ||
		pos.y < 0 ||
		pos.x > 10 - (isLookAtY ? 1 : shipSize) ||
		pos.y > 10 - (isLookAtY ? shipSize : 1))
		return false;

	POINT checkPos = pos;
	auto& upper = isLookAtY ? checkPos.y : checkPos.x;
	for (UINT i = 0; i < shipSize; i++)
	{
		if (targetMap->GetTile(checkPos).first)
			return false;
		upper++;
	}
	return true;
}

void InGame::ShipPlace(Ship * target, const POINT & pos, const bool & isLookAtY, const bool & byPlayer)
{
	UINT shipSize = target->GetSize();
	Map* targetMap = byPlayer ? m_playerMap : m_enemyMap;

	POINT checkPos = pos;
	auto& upper = isLookAtY ? checkPos.y : checkPos.x;
	for (UINT i = 0; i < shipSize; i++)
	{
		targetMap->GetTile(checkPos).first = target;
		upper++;
	}

	target->SetPlaceData(pos, isLookAtY);
}

void InGame::ShipRandomPlace(const bool & byPlayer)
{
	Map* targetMap = byPlayer ? m_playerMap : m_enemyMap;
	auto& shipList = byPlayer ? m_playerShip : m_enemyShip;

	for (UINT i = 0; i < 5; NULL)
	{
		if (byPlayer && m_isShipPlaced[i])
		{
			i++;
			continue;
		}

		Ship* target = shipList[i];
		POINT pos;
		pos.x = rand() % 10;
		pos.y = rand() % 10;
		bool isLookAtY = rand() % 1;

		if (IsShipCanPlace(target, pos, isLookAtY, byPlayer))
		{
			ShipPlace(target, pos, isLookAtY, byPlayer);
			i++;
		}
	}

	if (byPlayer)
	{
		m_isShipPlaced.fill(true);
		m_selectedItem = -1;
	}
}



Ship * InGame::GetRandomLiveShip(const bool& forPlayer)
{
	auto& shipList = forPlayer ? m_playerShip : m_enemyShip;

	UINT liveCount = 0;
	for (auto& ship : shipList)
	{
		if (ship->GetLife())
			liveCount++;
	}

	if (!liveCount)
		return nullptr;

	UINT index = rand() % liveCount;
	UINT result = 0;
	do
	{
		if (!shipList[result]->GetLife())
			result++;
		else if (index)
		{
			index--;
			result++;
		}
		else
			break;
	} while (true);

	return shipList[result];
}


void InGame::DefaultAttack(Ship * launcher, const POINT & pos, const bool & byPlayer)
{
	for (UINT i = 0; i < 8; i++)
		m_listLockon.push_back(new Lockon(launcher, pos, byPlayer, i * 5));
}

void InGame::LineAttack(Ship * launcher, const UINT & line, const bool & isLookAtY, const bool & byPlayer)
{
	POINT pos;
	pos.x = pos.y = 0;
	(isLookAtY ? pos.x : pos.y) = line;

	for (auto& upper = (isLookAtY ? pos.y : pos.x); upper < 10; upper++)
		m_listLockon.push_back(new Lockon(launcher, pos, byPlayer, upper * 5));
}

void InGame::RandomAttack(Ship * launcher, const bool & byPlayer)
{
	auto& targetMap = (byPlayer ? m_enemyMap : m_playerMap);

	UINT canAttackPosCount = 0;
	for (UINT i = 0; i < 100; i++)
	{
		if (!targetMap->GetTile(i).second)
			canAttackPosCount++;
	}

	if (!canAttackPosCount)
		return;
	else if (canAttackPosCount > 8)
		canAttackPosCount = 8;


	std::array<POINT, 8> prevAttack;
	prevAttack.fill([]()->POINT
	{POINT temp; temp.x = temp.y = -1; return temp; }());
	for (UINT i = 0; i < canAttackPosCount; NULL)
	{
		POINT pos;
		pos.x = rand() % 10;
		pos.y = rand() % 10;

		bool isSame = false;
		for (UINT prev = 0; prev < i; prev++)
		{
			if (prevAttack[prev].x == pos.x &&
				prevAttack[prev].y == pos.y)
			{
				isSame = true;
				return;
			}
		}
		cout << pos.x << " " << pos.y << endl;
		if (isSame || targetMap->GetTile(pos).second)
			continue;
		
		m_listLockon.push_back(new Lockon(launcher, pos, byPlayer, i * 5));
		i++;
	}
}

void InGame::_33Attack(Ship * launcher, const POINT & pos, const bool & byPlayer)
{
	for (int y = -1; y < 2; y++)
	{
		for (int x = -1; x < 2; x++)
		{
			POINT attackPos = pos;
			attackPos.x += x;
			attackPos.y += y;
			if (attackPos.x < 0 ||
				attackPos.y < 0 ||
				attackPos.x > 9 ||
				attackPos.y > 9)
				continue;

			m_listLockon.push_back(new Lockon(launcher, attackPos, byPlayer, ((x+1) + (y+1) * 3) * 5));
		}
	}
}

void InGame::Scanning(Ship * launcher, const POINT & pos, const bool & byPlayer)
{
	m_scanner = new Scanner(
		(byPlayer ? m_enemyMap : m_playerMap)->PointToVec(pos) + D3DXVECTOR3(0, 1, 0),
		pos,
		byPlayer);
}
