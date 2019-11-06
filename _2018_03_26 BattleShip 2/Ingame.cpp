#include "DXUT.h"
#include "Ingame.h"

#include "Map.h"
#include "Ship.h"
#include "GameUI.h"

#include "Lockon.h"
#include "Missile.h"
#include "Scan.h"

#include "ScanEffect.h"
#include "Flame.h"


Ingame::Ingame()
	: m_playerMap(nullptr)
{
}
Ingame::~Ingame()
{
}



void Ingame::Initialize()
{
	for (UINT i = 0; i < 10; i++)
		TextureLoad(L"./Resource/map/Click_" + std::to_wstring(i) + L".png", L"Click_" + std::to_wstring(i));


	TextureLoad(L"./Resource/ui/Ally_Turn.png",		L"MyTurn");
	TextureLoad(L"./Resource/ui/Enemy_Turn.png",	L"EnemyTurn");
	TextureLoad(L"./Resource/ui/Score.png",			L"ScoreUI");

	TextureLoad(L"./Resource/ui/Defeat.png", L"Victory_E");
	TextureLoad(L"./Resource/ui/Victoru.png", L"Victory_P");

	ASELoad(L"./Resource/model", L"T2.ase",		L"T2");
	ASELoad(L"./Resource/model", L"T3_0.ase",	L"T3");
	ASELoad(L"./Resource/model", L"T3_1.ase",	L"T3_1");
	ASELoad(L"./Resource/model", L"T4.ase",		L"T4");

	ASELoad(L"./Resource/model", L"Missile.ase", L"Missile");



	SingletonInstance(SoundManager)->Load(L"./Resource/snd/Ingame.wav", L"BGM_Ingame");
	SingletonInstance(SoundManager)->Load(L"./Resource/snd/Win.wav", L"VICTORY_P");
	SingletonInstance(SoundManager)->Load(L"./Resource/snd/Win.wav", L"VICTORY_E");

	SingletonInstance(SoundManager)->Load(L"./Resource/snd/Check.wav",			L"Lockon"		, 9);
	SingletonInstance(SoundManager)->Load(L"./Resource/snd/Destroy.wav",		L"Attack_T"		, 9);
	SingletonInstance(SoundManager)->Load(L"./Resource/snd/Destroy_Failed.wav",	L"Attack_F"		, 9);
	SingletonInstance(SoundManager)->Load(L"./Resource/snd/Select.wav",			L"SelectItem"	, 4);
	SingletonInstance(SoundManager)->Load(L"./Resource/snd/Shoot.wav",			L"Attack"		, 9);
	SingletonInstance(SoundManager)->Load(L"./Resource/snd/Tile_Select.wav",	L"SelectTile"	, 16);


	m_bgm = SingletonInstance(SoundManager)->Play(L"BGM_Ingame");


	/* 카메라 */
	m_camDistance = 30;
	m_camObjDist = 30;
	m_camRotX = 20;
	m_camRotY = 0;



	//맵 초기화
	m_playerMap = new Map();
	m_playerMap->ZeroMap();
	m_enemyMap = new Map();
	m_enemyMap->ZeroMap();

	//배 초기화
	m_playerShip[0] = new Ship(2);
	m_playerShip[1] = new Ship(2);
	m_playerShip[2] = new Ship(3);
	m_playerShip[3] = new Ship(3);
	m_playerShip[4] = new Ship(4);
	m_enemyShip[0] = new Ship(2);
	m_enemyShip[1] = new Ship(2);
	m_enemyShip[2] = new Ship(3);
	m_enemyShip[3] = new Ship(3);
	m_enemyShip[4] = new Ship(4);

	//UI 초기화
	m_playerUI = new GameUI(true);
	m_enemyUI = new GameUI(false);

	m_playerItem.fill(true);
	m_enemyItem.fill(true);


	m_scan = nullptr;


	m_shipPlaced.fill(false);



	/* 적 자동 배치 */ {
		for (UINT i = 0; i < 5;)
		{
			Ship*& nowShip = m_enemyShip[i];
			bool lookAtY = rand() & 0b1;
			POINT randomPos;
			randomPos.x = rand() % (10 + 1 - (lookAtY ? 1 : nowShip->GetSize()));
			randomPos.y = rand() % (10 + 1 - (lookAtY ? nowShip->GetSize() : 1));

			if (m_enemyMap->RegistShip(nowShip, randomPos, lookAtY))
			{
				nowShip->Placed(randomPos, lookAtY);
				i++;
			}
		}
	}



	m_isPlaying = false;



	//컨트롤러 초기화
	m_myCursorPos.x = m_myCursorPos.y = -1;
	m_myCursorShip = -1;
	m_myDirY = false;
	m_settingSkillMouseOn = -1;


	m_cursorInPlayerMap = true;
	m_itemSelect = 0;
	m_itemMouseOn = 0;

	m_isPlayerTurn = true;
	m_isTurnAttacked = false;
	m_turnStay = 0;
	m_turnFrame = 1000;








	m_gameEnd = 0;
	m_playerWin = false;
}

void Ingame::Update()
{
	SingletonInstance(SkySphere)->Update();
	if (SingletonInstance(Fader)->Update() && SingletonInstance(Fader)->IsIn())
	{
		SingletonInstance(Fader)->Create(false, 30);

		if (!m_isPlaying)
		{
			m_isPlaying = true;

			m_playerMap->SetMapPos(D3DXVECTOR3(-5 - 2, 0, 0));
			m_enemyMap->SetMapPos(D3DXVECTOR3(+5 + 2, 0, 0));
		}

		if (m_gameEnd)
		{
			SingletonInstance(SceneManager)->ChangeScene(L"MainMenu");
			if (m_bgm)
				m_bgm->Stop();
			return;
		}
	}
	if (m_bgm && !m_bgm->IsPlaying())
		m_bgm->Play();

	if (SingletonInstance(InputManager)->IsKeyDown('Z'))
		m_gameEnd = 1;

	CameraControl();

	if (m_gameEnd)
	{
		m_gameEnd++;
		
		if (SingletonInstance(InputManager)->IsKeyDown(VK_SPACE))
			SingletonInstance(Fader)->Create(true);
	}


	//준비 단계
	if (!m_isPlaying)
	{
		//배 선택
		SelectCursorShip();
		
		//배치 방향 변경
		if (SingletonInstance(InputManager)->IsKeyDown(VK_RBUTTON))	m_myDirY = !m_myDirY;

		//특수
		auto BtnOnMouse = [](const D3DXVECTOR2& focus, const float& radius)
		{
			POINT mousePos = SingletonInstance(InputManager)->GetMousePos();
			return D3DXVec2LengthSq(&(focus - D3DXVECTOR2(mousePos.x, mousePos.y))) < radius * radius;
		};

			 if (BtnOnMouse(D3DXVECTOR2(98, 846), 22))	{ m_settingSkillMouseOn = 0; if (SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)) ResetPlaceShip();	}
		else if (BtnOnMouse(D3DXVECTOR2(164, 846), 22))	{ m_settingSkillMouseOn = 1; if (SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)) AutoPlaceShip();	}
		else if (BtnOnMouse(D3DXVECTOR2(227, 846), 22))	{ m_settingSkillMouseOn = 2; }
		else											m_settingSkillMouseOn = -1;
		
		//배 설치

		POINT prevCursor = m_myCursorPos;
		if (m_playerMap->RayCastInTile(m_myCursorPos))
		{
			if (prevCursor.x != m_myCursorPos.x && prevCursor.y != m_myCursorPos.y)
				SingletonInstance(SoundManager)->Play(L"SelectTile");

			if (CursorShip())
			{
				auto& xOrY = m_myDirY ? m_myCursorPos.y : m_myCursorPos.x;
				if (xOrY >= 10 - CursorShip()->GetSize())
					xOrY = 10 - CursorShip()->GetSize();

				if (SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON) &&
					m_playerMap->RegistShip(CursorShip(), m_myCursorPos, m_myDirY))
				{
					CursorShip()->Placed(m_myCursorPos, m_myDirY);
					m_shipPlaced[m_myCursorShip] = true;
					m_myCursorShip = -1;
					m_myDirY = false;
				}
			}
		}
		
		/* 게임 시작 */ {
			if (m_settingSkillMouseOn == 2 && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON))
			{
				bool startReady = true;
				for (auto& isShipPlaced : m_shipPlaced)
				{
					if (!isShipPlaced)
					{
						startReady = false;
						break;
					}
				}
				if (startReady)
				{
					if (!SingletonInstance(Fader)->IsRun())
						SingletonInstance(Fader)->Create(true);
				}
			}
		}
	}
	//시작
	else
	{
		m_playerUI->Update();
		m_enemyUI->Update();

		m_turnFrame++;

		// Scan
		if (m_scan)
		{
			m_scan->Update();
			if (m_scan->GetFrame() > 180)
				m_scan->Expire();

			if (m_scan->IsExpired())
			{
				for (int x = -1; x < 2; x++)
				{
					for (int y = -1; y < 2; y++)
					{
						POINT tilePos = m_scan->GetLockPos();
						tilePos.x += x;
						tilePos.y += y;

						if (tilePos.x < 0 ||
							tilePos.y < 0 ||
							10 <= tilePos.x ||
							10 <= tilePos.y)
							continue;

						Map* map = (m_scan->IsPlayerAttack() ? m_enemyMap : m_playerMap);
						if (map->GetTile(tilePos).first)
							m_listEffect.push_back(new ScanEffect(map->PointToVector(tilePos)));
					}
				}
				m_turnStay = 60;

				SAFE_DELETE( m_scan );
			}
		}

		// Lockon 연산
		for (auto& lockon = m_listLockon.begin(); lockon != m_listLockon.end();)
		{
			(*lockon)->Update();
			if ((*lockon)->IsExpired())
			{
				//미사일 생산
				Map* mslMap			= (*lockon)->IsPlayerAttack() ? m_playerMap : m_enemyMap;

				D3DXVECTOR3 mslPos = mslMap->PointToVector(0, 0);
				D3DXVECTOR3 mslObjPos = mslPos + D3DXVECTOR3(0, 50, 0);
				
				Missile* msl = new Missile((*lockon)->GetLockPos(), (*lockon)->IsPlayerAttack());
				msl->SetPath(mslPos, []()->D3DXVECTOR3
				{
					D3DXVECTOR3 dir;
					D3DXMATRIX rm;
					D3DXMatrixRotationY(&rm, D3DXToRadian(rand()%3600 / 10.));
					D3DXVec3TransformNormal(&dir, &D3DXVECTOR3(0, 0, 1), &rm);
					return rm;
				}
				(), mslObjPos);
				m_listMissile.push_back(msl);
				SingletonInstance(SoundManager)->Play(L"Attack");


				SAFE_DELETE(*lockon);
				lockon = m_listLockon.erase(lockon);
			}
			else
				lockon++;
		}

		// Missile 연산
		for (auto& missile = m_listMissile.begin(); missile != m_listMissile.end();)
		{
			(*missile)->Update();
			if ((*missile)->IsByPlayer() || (*missile)->IsFalling())
			{
				m_listEffect.push_back(new Flame((*missile)->GetPos()));
			}

			if (!(*missile)->IsFalling())
			{
				if ((*missile)->GetPos().y > 40)
				{
					Map* mslEnemyMap	= (*missile)->IsByPlayer() ? m_enemyMap : m_playerMap;

					D3DXVECTOR3 mslTerminal = mslEnemyMap->PointToVector((*missile)->GetLockonPos());
					D3DXVECTOR3 mslPos = mslTerminal + D3DXVECTOR3(0, 50, 0);
					(*missile)->SetPath(mslPos, D3DXVECTOR3(0, -1, 0), mslTerminal);
					(*missile)->Falling();
				}
			}
			else
			{
				if ((*missile)->GetPos().y < 0)
					(*missile)->Expire();
			}


			if ((*missile)->IsExpired())
			{
				//맵에 데이터 저장
				Map* mslEnemyMap = (*missile)->IsByPlayer() ? m_enemyMap : m_playerMap;

				POINT tilePos = (*missile)->GetLockonPos();
				if (!mslEnemyMap->GetTile(tilePos).second)
				{
					if (mslEnemyMap->Hit(tilePos))
						SingletonInstance(SoundManager)->Play(L"Attack_T");
					else
						SingletonInstance(SoundManager)->Play(L"Attack_F");
					m_turnStay = 60;
				}


				SAFE_DELETE(*missile);
				missile = m_listMissile.erase(missile);
			}
			else
				missile++;
		}
		//턴 넘김 (Missile 충돌 연산의 연장선)
		{
			if (m_turnStay && m_listMissile.empty())
			{
				m_turnStay--;
				if (!m_turnStay)
				{
					m_isPlayerTurn = !m_isPlayerTurn;
					m_isTurnAttacked = false;

					m_turnFrame = 0;
				}
			}
		}

		// Effect 연산
		for (auto& effect = m_listEffect.begin(); effect != m_listEffect.end();)
		{
			(*effect)->Update();
			if ((*effect)->IsExpired())
			{
				SAFE_DELETE(*effect);
				effect = m_listEffect.erase(effect);
			}
			else
				effect++;
		}


		//커서 맵 충돌 체크
		bool cursorInMap = false;
		POINT prevCursor = m_myCursorPos;
		if (m_playerMap->RayCastInTile(m_myCursorPos))
		{
			cursorInMap = true;
			m_cursorInPlayerMap = true;
		}
		else if (m_enemyMap->RayCastInTile(m_myCursorPos))
		{
			cursorInMap = true;
			m_cursorInPlayerMap = false;
		}

		if (m_cursorInPlayerMap && prevCursor.x != m_myCursorPos.x && prevCursor.y != m_myCursorPos.y)
			SingletonInstance(SoundManager)->Play(L"SelectTile");

		/* 아이템 선택 */ {
			UINT prevItem = m_itemSelect;

			auto BtnClick = [](const D3DXVECTOR2& focus, const D3DXVECTOR2& size)
			{
				POINT mousePos = SingletonInstance(InputManager)->GetMousePos();
				return 
					focus.x - size.x < mousePos.x &&
					focus.y - size.y < mousePos.y &&
					mousePos.x < focus.x + size.x &&
					mousePos.y < focus.y + size.y;
			};
				 if (m_playerItem[0] && BtnClick(D3DXVECTOR2(69, 792 ), D3DXVECTOR2(30, 30)))	{ m_itemMouseOn = 0; if (SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)) m_itemSelect = 1; }
			else if (m_playerItem[1] && BtnClick(D3DXVECTOR2(133, 792), D3DXVECTOR2(30, 30)))	{ m_itemMouseOn = 1; if (SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)) m_itemSelect = 2; }
			else if (m_playerItem[2] && BtnClick(D3DXVECTOR2(196, 792), D3DXVECTOR2(30, 30)))	{ m_itemMouseOn = 2; if (SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)) m_itemSelect = 3; }
			else if (m_playerItem[3] && BtnClick(D3DXVECTOR2(258, 792), D3DXVECTOR2(30, 30)))	{ m_itemMouseOn = 3; if (SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)) m_itemSelect = 4; }
			else															m_itemMouseOn = -1;

			auto BtnOnMouse = [](const D3DXVECTOR2& focus, const float& radius)
			{
				POINT mousePos = SingletonInstance(InputManager)->GetMousePos();
				return D3DXVec2LengthSq(&(focus - D3DXVECTOR2(mousePos.x, mousePos.y))) < radius * radius;
			};
				 if (BtnOnMouse(D3DXVECTOR2(164, 846), 22))	{ m_settingSkillMouseOn = 0; if (SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)) m_itemSelect = 0; }
			else											m_settingSkillMouseOn = -1;


			if (prevItem != m_itemSelect)
				SingletonInstance(SoundManager)->Play(L"SelectItem");
		}

		//선택 및 발사
		if (!m_isTurnAttacked && !m_gameEnd)
		{
			if (m_isPlayerTurn)
			{
				switch (m_itemSelect)
				{
				case 0:
					if (SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)
						&& cursorInMap && !m_cursorInPlayerMap)
					{
						if (!m_enemyMap->GetTile(m_myCursorPos).second)
						{
							m_listLockon.push_back(new Lockon(m_myCursorPos, true));
							m_isTurnAttacked = true;
						}
					}
					break;

				case 1:
					if (SingletonInstance(InputManager)->IsKeyDown(VK_RBUTTON))
						m_myDirY = !m_myDirY;

					if (SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)
						&& cursorInMap && !m_cursorInPlayerMap)
						LineAttack(true);
					break;

				case 2:
					if (SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)
						&& cursorInMap && !m_cursorInPlayerMap)
						RandomAttack(true);
					break;

				case 3:
					if (SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)
						&& cursorInMap && !m_cursorInPlayerMap)
						_33Attack(true);
					break;

				case 4:
					if (SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)
						&& cursorInMap && !m_cursorInPlayerMap)
						Scanner(true);
					break;
				}
			}
			else
			{
				POINT pos;
				pos.x = rand() % 10;
				pos.y = rand() % 10;

				if (rand() % 3 == 0)
				{
					UINT item = rand() % 4;
					if (m_enemyItem[item])
					{
						switch (item)
						{
						case 0:		LineAttack(false);	break;
						case 1:		RandomAttack(false); break;
						case 2:		Scanner(false);		break;
						case 3:		_33Attack(false);	break;
						}
					}
				}
				else if (!m_playerMap->GetTile(pos).second)
				{
					m_listLockon.push_back(new Lockon(pos, false));
					m_isTurnAttacked = true;
				}
			}
		}


		//게임끝 체크
		if (!m_gameEnd)
		{
			bool playerWin = true;
			bool enemyWin = true;
			for (auto& ship : m_playerShip)
			{
				if (ship->GetLife())
				{
					enemyWin = false;
					break;
				}
			}
			for (auto& ship : m_enemyShip)
			{
				if (ship->GetLife())
				{
					playerWin = false;
					break;
				}
			}

			if (playerWin)
			{
				m_gameEnd = 1;
				m_playerWin = true;

				if (m_bgm)
					m_bgm->Stop();
				m_bgm = SingletonInstance(SoundManager)->Play(L"VICTORY_P");
			}
			else if (enemyWin)
			{
				m_gameEnd = 1;
				m_playerWin = false;

				if (m_bgm)
					m_bgm->Stop();
				m_bgm = SingletonInstance(SoundManager)->Play(L"VICTORY_E");
			}
		}
	}
}

void Ingame::Render()
{
	SingletonInstance(SkySphere)->Render();

	//준비단계
	if (!m_isPlaying)
	{
		/* Render Placed Ship */ {
			for (UINT i = 0; i < 5; i++)
			{
				if (!m_shipPlaced[i])
					continue;

				Ship* ship = m_playerShip[i];
				D3DXMATRIX tm; {
					D3DXVECTOR3 pos = m_playerMap->PointToVector(ship->GetTilePos());
					D3DXMatrixTranslation(&tm, pos.x, pos.y, pos.z);
				}
				D3DXMATRIX rm; {
					D3DXMatrixRotationY(&rm, D3DXToRadian(ship->GetLookAtY() ? 0 : 90));
				}
				ship->Render(rm * tm);
			}
		}

		/* Default Map */
		m_playerMap->RenderMap();

		/* Select Tile */
		if (m_myCursorPos.x != -1)
		{
			if (CursorShip())
			{
				POINT _pos = m_myCursorPos;
				auto& xOrY = m_myDirY ? _pos.y : _pos.x;
				for (UINT i = 0; i < CursorShip()->GetSize(); i++)
				{
					bool cannotPlaceThere = m_playerMap->GetTile(_pos).first;
					m_playerMap->RenderTile(_pos, cannotPlaceThere ? TextureGet(L"Tile_F") : nullptr);
					xOrY++;
				}
			}
			else
				m_playerMap->RenderTile(m_myCursorPos);
		}

		/* UI */ {
			m_playerUI->RenderBegin();

			m_playerUI->RenderBase(D3DXVECTOR2(13, 143));

			if (!m_shipPlaced[0])	m_playerUI->RenderShip(D3DXVECTOR2(35, 263),	2, false, m_myCursorShip != 0);
			if (!m_shipPlaced[1])	m_playerUI->RenderShip(D3DXVECTOR2(171, 263),	2, false, m_myCursorShip != 1);
			if (!m_shipPlaced[2])	m_playerUI->RenderShip(D3DXVECTOR2(35, 383),	3, false, m_myCursorShip != 2);
			if (!m_shipPlaced[3])	m_playerUI->RenderShip(D3DXVECTOR2(35, 503),	3, false, m_myCursorShip != 3);
			if (!m_shipPlaced[4])	m_playerUI->RenderShip(D3DXVECTOR2(35, 621),	4, false, m_myCursorShip != 4);
			
			m_playerUI->RenderItem(D3DXVECTOR2(69, 792)		, 0, false);
			m_playerUI->RenderItem(D3DXVECTOR2(133, 792)	, 1, false);
			m_playerUI->RenderItem(D3DXVECTOR2(196, 792)	, 2, false);
			m_playerUI->RenderItem(D3DXVECTOR2(258, 792)	, 3, false);

			m_playerUI->RenderBtn(D3DXVECTOR2(74, 823 )	, 0, m_settingSkillMouseOn == 0);
			m_playerUI->RenderBtn(D3DXVECTOR2(141, 823)	, 1, m_settingSkillMouseOn == 1);
			m_playerUI->RenderBtn(D3DXVECTOR2(204, 823)	, 2, m_settingSkillMouseOn == 2);
			if (m_settingSkillMouseOn != -1)
				m_playerUI->RenderBtnInfo(m_settingSkillMouseOn);

			m_playerUI->RenderEnd();
		}
	}
	//시작
	else
	{
		/* Render Live Player Ship */ {
			for (auto& playerShip : m_playerShip)
			{
				if (!playerShip->GetLife())
					continue;

				D3DXMATRIX tm; {
					D3DXVECTOR3 pos = m_playerMap->PointToVector(playerShip->GetTilePos());
					D3DXMatrixTranslation(&tm, pos.x, pos.y, pos.z);
				}
				D3DXMATRIX rm; {
					D3DXMatrixRotationY(&rm, D3DXToRadian(playerShip->GetLookAtY() ? 0 : 90));
				}
				playerShip->Render(rm * tm);
			}
		}

		/* Missile */ {
			for (auto& missile : m_listMissile)
				missile->Render();
		}

		/* Default Map */
		m_playerMap->RenderMap();
		m_enemyMap->RenderMap();

		/* Select Tile */
		switch (m_itemSelect)
		{
		case 0:
		case 2:
			if (m_myCursorPos.x != -1 && !m_cursorInPlayerMap)
				m_enemyMap->RenderTile(m_myCursorPos);
			break;

		case 1:
			if (m_myCursorPos.x != -1 && !m_cursorInPlayerMap)
			{
				UINT line = m_myDirY ? m_myCursorPos.x : m_myCursorPos.y;

				for (UINT i = 0; i < 10; i++)
				{
					POINT pos;
					(m_myDirY ? pos.y : pos.x) = i;
					(m_myDirY ? pos.x : pos.y) = line;

					m_enemyMap->RenderTile(pos);
				}
			}
			break;

		case 3:
		case 4:
			if (m_myCursorPos.x != -1 && !m_cursorInPlayerMap)
			{
				for (int x = -1; x < 2; x++)
				{
					for (int y = -1; y < 2; y++)
					{
						POINT tilePos = m_myCursorPos;
						tilePos.x += x;
						tilePos.y += y;

						if (tilePos.x < 0 ||
							tilePos.y < 0 ||
							10 <= tilePos.x ||
							10 <= tilePos.y)
							continue;

						m_enemyMap->RenderTile(tilePos);
					}
				}
			}
			break;
		}

		// Lockoner 
		for (auto& lockon : m_listLockon)
		{
			if (!lockon->IsStay())
			{
				D3DXMATRIX bill; {
					DEVICE->GetTransform(D3DTS_VIEW, &bill);
					D3DXMatrixInverse(&bill, 0, &bill);
					bill._41 = bill._42 = bill._43 = 0;
				}
				D3DXMATRIX sm; {
					constexpr FLOAT SCALE = 1.5;
					D3DXMatrixScaling(&sm, SCALE, SCALE, SCALE);
				}
				D3DXMATRIX tm; {
					D3DXVECTOR3 thisPos = (lockon->IsPlayerAttack() ? m_enemyMap : m_playerMap)->PointToVector(lockon->GetLockPos());
					D3DXMatrixTranslation(&tm, thisPos.x, thisPos.y, thisPos.z);
				}
				
				
				DEVICE->SetFVF(Plane::GetFVF());
				DEVICE->SetTexture(0, lockon->GetTexture()->tex);
				DEVICE->SetTransform(D3DTS_WORLD, &(bill * sm * tm));
				DEVICE->SetRenderState(D3DRS_ZENABLE, false);
				DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, false);
				SingletonInstance(Plane)->Render(DEVICE);
				DEVICE->SetRenderState(D3DRS_ZENABLE, true);
				DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);
			}
		}

		if (m_scan)m_scan->Render();
		

		/* Effect */
		for (auto& effect : m_listEffect)
			effect->Render();
			

		/* UI */ {
			m_playerUI->RenderBegin();

			m_playerUI->RenderBase(D3DXVECTOR2(13, 143));

			m_playerUI->RenderShip(D3DXVECTOR2(35, 263),	2, !m_playerShip[0]->GetLife());
			m_playerUI->RenderShip(D3DXVECTOR2(171, 263),	2, !m_playerShip[1]->GetLife());
			m_playerUI->RenderShip(D3DXVECTOR2(35, 383),	3, !m_playerShip[2]->GetLife());
			m_playerUI->RenderShip(D3DXVECTOR2(35, 503),	3, !m_playerShip[3]->GetLife());
			m_playerUI->RenderShip(D3DXVECTOR2(35, 621),	4, !m_playerShip[4]->GetLife());

			m_playerUI->RenderItem(D3DXVECTOR2(69, 792),	0, m_itemSelect == 1 || m_itemMouseOn == 0);
			m_playerUI->RenderItem(D3DXVECTOR2(133, 792),	1, m_itemSelect == 2 || m_itemMouseOn == 1);
			m_playerUI->RenderItem(D3DXVECTOR2(196, 792),	2, m_itemSelect == 3 || m_itemMouseOn == 2);
			m_playerUI->RenderItem(D3DXVECTOR2(258, 792),	3, m_itemSelect == 4 || m_itemMouseOn == 3);

			m_playerUI->RenderBtn(D3DXVECTOR2(141, 823),	0, m_settingSkillMouseOn == 0);
			if (m_settingSkillMouseOn != -1)
				m_playerUI->RenderBtnInfo(m_settingSkillMouseOn);

			m_playerUI->RenderEnd();


			
			m_enemyUI->RenderBegin();

			m_enemyUI->RenderBase(D3DXVECTOR2(1600 - 13, 143));

			m_enemyUI->RenderShip(D3DXVECTOR2(1600 - 35, 263),		2, !m_enemyShip[0]->GetLife());
			m_enemyUI->RenderShip(D3DXVECTOR2(1600 - 171, 263),	2, !m_enemyShip[1]->GetLife());
			m_enemyUI->RenderShip(D3DXVECTOR2(1600 - 35, 383),		3, !m_enemyShip[2]->GetLife());
			m_enemyUI->RenderShip(D3DXVECTOR2(1600 - 35, 503),		3, !m_enemyShip[3]->GetLife());
			m_enemyUI->RenderShip(D3DXVECTOR2(1600 - 35, 621),		4, !m_enemyShip[4]->GetLife());

			m_enemyUI->RenderItem(D3DXVECTOR2(1600 - 69, 792),		0, m_enemyItem[0]);
			m_enemyUI->RenderItem(D3DXVECTOR2(1600 - 133, 792),	1, m_enemyItem[1]);
			m_enemyUI->RenderItem(D3DXVECTOR2(1600 - 196, 792),	2, m_enemyItem[2]);
			m_enemyUI->RenderItem(D3DXVECTOR2(1600 - 258, 792),	3, m_enemyItem[3]);

			m_enemyUI->RenderEnd();
		}

		/* Turn */ {
			SPRITE->Begin(D3DXSPRITE_ALPHABLEND);
			Texture* tex = TextureGet(!m_isPlayerTurn ? L"EnemyTurn" : L"MyTurn");
			SPRITE->Draw(tex->tex, nullptr, &D3DXVECTOR3(tex->info.Width * 0.5f, 0, 0), &D3DXVECTOR3(800, 0, 0), D3DXCOLOR(1, 1, 1, 1));
			SPRITE->End();
		}

		/* Score */ {
			SPRITE->Begin(D3DXSPRITE_ALPHABLEND);
			
			Texture* tex = TextureGet(L"ScoreUI");
			SPRITE->Draw(tex->tex, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, 1));
			
			SPRITE->End();
		}
	}

	if (m_gameEnd)
	{
		Texture* tex = TextureGet(m_playerWin ? L"Victory_P" : L"Victory_E");
		float scale = m_gameEnd < 15 ?
			sinf(D3DXToRadian(m_gameEnd * 6)) :
			1;

		D3DXMATRIX sm, tm;
		D3DXMatrixScaling(&sm, scale, scale, scale);
		D3DXMatrixTranslation(&tm, 800, 450, 0);

		SPRITE->Begin(D3DXSPRITE_ALPHABLEND);
		SPRITE->SetTransform(&(sm * tm));
		SPRITE->Draw(tex->tex, nullptr, &D3DXVECTOR3(tex->info.Width * 0.5, tex->info.Height * .5f, 0), nullptr, D3DXCOLOR(1, 1,1, 1));
		SPRITE->End();
	}

	SingletonInstance(Fader)->Render();
}

void Ingame::Release()
{
	for (auto& lockon : m_listLockon)	SAFE_DELETE(lockon);	m_listLockon.clear();
	for (auto& missile : m_listMissile)	SAFE_DELETE(missile);	m_listMissile.clear();
	for (auto& effect : m_listEffect)	SAFE_DELETE(effect);	m_listEffect.clear();


	SAFE_DELETE( m_playerShip[0] );
	SAFE_DELETE( m_playerShip[1] );
	SAFE_DELETE( m_playerShip[2] );
	SAFE_DELETE( m_playerShip[3] );
	SAFE_DELETE( m_playerShip[4] );
	SAFE_DELETE( m_enemyShip[0] );
	SAFE_DELETE( m_enemyShip[1] );
	SAFE_DELETE( m_enemyShip[2] );
	SAFE_DELETE( m_enemyShip[3] );
	SAFE_DELETE( m_enemyShip[4] );
	
	SAFE_DELETE(m_playerMap);
	SAFE_DELETE(m_enemyMap);

	SAFE_DELETE(m_playerUI);
	SAFE_DELETE(m_enemyUI);

	SAFE_DELETE(m_scan);
}



void Ingame::CameraControl()
{
	if (SingletonInstance(InputManager)->IsKeyPressed(VK_MBUTTON))
	{
		m_camRotX += SingletonInstance(InputManager)->GetMouseDelta().y;
		m_camRotY += SingletonInstance(InputManager)->GetMouseDelta().x;
		if (m_camRotX < 15) m_camRotX = 15;
		if (m_camRotX >= 90) m_camRotX = 90 - 0.0001;
	}
	m_camObjDist -= (float)SingletonInstance(InputManager)->GetMouseWheel() / 120;
	if (m_camObjDist < 2) m_camObjDist = 2;
	if (m_camObjDist >= 50) m_camObjDist = 50;
	
	m_camDistance = m_camDistance + (m_camObjDist - m_camDistance) * 0.1f;

	D3DXMATRIX rXM, rYM;
	D3DXMatrixRotationX(&rXM, D3DXToRadian(m_camRotX));
	D3DXMatrixRotationY(&rYM, D3DXToRadian(m_camRotY));

	D3DXVECTOR3 camPos;
	D3DXVec3TransformNormal(&camPos, &D3DXVECTOR3(0, 0, -m_camDistance), &(rXM * rYM));
	SingletonInstance(Camera)->Transform(&camPos, &D3DXVECTOR3(0, 0, 0));
}



void Ingame::SelectCursorShip()
{
	POINT mousePos = SingletonInstance(InputManager)->GetMousePos();
	auto BtnClick = [&](const D3DXVECTOR2& lt, const D3DXVECTOR2& pls)->bool
	{
		return
			lt.x < mousePos.x &&
			lt.y < mousePos.y &&
			mousePos.x < lt.x + pls.x &&
			mousePos.y < lt.y + pls.y;
	};

	if (BtnClick(D3DXVECTOR2(35, 263)	, D3DXVECTOR2(123, 110)) && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)) { m_myCursorShip = m_shipPlaced[0] ? -1 : 0; }
	if (BtnClick(D3DXVECTOR2(171, 263)	, D3DXVECTOR2(123, 110)) && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)) { m_myCursorShip = m_shipPlaced[1] ? -1 : 1; }
	if (BtnClick(D3DXVECTOR2(35, 383)	, D3DXVECTOR2(188, 110)) && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)) { m_myCursorShip = m_shipPlaced[2] ? -1 : 2; }
	if (BtnClick(D3DXVECTOR2(35, 503)	, D3DXVECTOR2(188, 110)) && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)) { m_myCursorShip = m_shipPlaced[3] ? -1 : 3; }
	if (BtnClick(D3DXVECTOR2(35, 621)	, D3DXVECTOR2(257, 110)) && SingletonInstance(InputManager)->IsKeyDown(VK_LBUTTON)) { m_myCursorShip = m_shipPlaced[4] ? -1 : 4; }
}

void Ingame::AutoPlaceShip()
{
	for (UINT i = 0; i < 5;)
	{
		if (m_shipPlaced[i])
		{
			i++;
			continue;
		}

		Ship*& nowShip = m_playerShip[i];
		bool lookAtY = rand() & 0b1;
		POINT randomPos;
		randomPos.x = rand() % ( 10 + 1 - (lookAtY ? 1 : nowShip->GetSize()) );
		randomPos.y = rand() % ( 10 + 1 - (lookAtY ? nowShip->GetSize() : 1) );

		if (m_playerMap->RegistShip(nowShip, randomPos, lookAtY))
		{
			nowShip->Placed(randomPos, lookAtY);
			m_shipPlaced[i] = true;
			i++;
			continue;
		}
	}

	m_myCursorShip = -1;
	m_myDirY = false;
}

void Ingame::ResetPlaceShip()
{
	m_playerMap->ZeroMap();
	m_shipPlaced.fill(false);
}



void Ingame::LineAttack(const bool& byPlayer)
{
	if (m_isPlayerTurn && m_enemyMap->GetTile(m_myCursorPos).second)
		return;

	Map* target = byPlayer ? m_enemyMap : m_playerMap;

	bool lookAtY = byPlayer ?
		m_myDirY :
		rand() & 0b1;

	UINT line = byPlayer ?
		(lookAtY ? m_myCursorPos.x : m_myCursorPos.y) :
		rand() % 10;


	for (UINT i = 0; i < 10; i++)
	{
		POINT pos;
		(lookAtY ? pos.y : pos.x) = i;
		(lookAtY ? pos.x : pos.y) = line;

		m_listLockon.push_back(new Lockon(pos, true, i * 5));
	}
	(byPlayer ? m_playerItem : m_enemyItem)[0] = false;

	m_itemSelect = 0;
	m_isTurnAttacked = true;
}

void Ingame::RandomAttack(const bool& byPlayer)
{
	if (m_isPlayerTurn && m_enemyMap->GetTile(m_myCursorPos).second)
		return;


	Map* target = byPlayer ? m_enemyMap : m_playerMap;

	UINT maxAttack = 0;
	for (UINT i = 0; i < 100; i++)
	{
		if (!target->GetTile(i).second)
			maxAttack++;
	}
	if (maxAttack > 8)
		maxAttack = 8;

	std::array<POINT, 8> prev;
	POINT temp; temp.x = temp.y = -1;
	prev.fill(temp);

	for (UINT i = 0; i < maxAttack;)
	{
		POINT pos;
		pos.x = rand() % 10;
		pos.y = rand() % 10;
		
		bool isOk = true;
		for (UINT j = 0; j < i; j++)
		{
			if (prev[j].x == pos.x && prev[j].y == pos.y)
			{
				isOk = false;
				break;
			}
		}

		if (isOk && !target->GetTile(pos).second)
		{
			prev[i] = pos;
			m_listLockon.push_back(new Lockon(pos, byPlayer, i * 10));
			i++;
		}
	}

	(byPlayer ? m_playerItem : m_enemyItem)[1] = false;
	m_itemSelect = 0;
	m_isTurnAttacked = true;
}

void Ingame::_33Attack(const bool & byPlayer)
{
	if (m_isPlayerTurn && m_enemyMap->GetTile(m_myCursorPos).second)
		return;


	Map* target = byPlayer ? m_enemyMap : m_playerMap;
	POINT pos;
	if (byPlayer)
		pos = m_myCursorPos;
	else
	{
		pos.x = rand() % 10;
		pos.y = rand() % 10;
	}

	std::vector<POINT> attackPosList;
	attackPosList.resize(9);
	for (auto& iter : attackPosList) iter.x = iter.y = -1;

	UINT leftPlace = 9;
	for (int x = -1; x < 2; x++)
	{
		for (int y = -1; y < 2; y++)
		{
			POINT attackPos = pos;
			attackPos.x += x;
			attackPos.y += y;

			if (attackPos.x < 0 ||
				attackPos.y < 0 ||
				10 <= attackPos.x ||
				10 <= attackPos.y)
			{
				leftPlace--;
				continue;
			}

			UINT count = rand() % leftPlace;
			UINT place = 0;
			do
			{
				if (attackPosList[place].x != -1)
					place++;
				else if (count)
				{
					count--;
					place++;
				}
				else
					break;
			} while (true);

			attackPosList[place] = attackPos;
			leftPlace--;
		}
	}

	for (UINT i = 0; i < 9; i++)
	{
		if (attackPosList[i].x == -1)
			continue;

		m_listLockon.push_back(new Lockon(attackPosList[i], true, i * 5));
		m_isTurnAttacked = true;
	}

	(byPlayer ? m_playerItem : m_enemyItem)[2] = false;
	m_itemSelect = 0;
	m_isTurnAttacked = true;
}

void Ingame::Scanner(const bool & byPlayer)
{
	if (m_isPlayerTurn && m_enemyMap->GetTile(m_myCursorPos).second)
		return;


	Map* target = byPlayer ? m_enemyMap : m_playerMap;
	POINT pos;
	if (byPlayer)
		pos = m_myCursorPos;
	else
	{
		pos.x = rand() % 10;
		pos.y = rand() % 10;
	}


	m_scan = new Scan(target->PointToVector(pos) + D3DXVECTOR3(0, 2, 0), pos, byPlayer);

	(byPlayer ? m_playerItem : m_enemyItem)[3] = false;

	m_itemSelect = 0;
	m_isTurnAttacked = true;
}
