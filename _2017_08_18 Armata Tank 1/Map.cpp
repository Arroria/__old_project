#include "MainHeader.h"
#include "Map.h"

#include "ASE.h"
#include "MapManager.h"
#include "EnemyManager.h"
#include "Enemy.h"
#include "ItemManager.h"

#define MAPTILESIZE 50

namespace AF
{
	Map::Map(MapManager* mapMgr, const int & type, const int& mapEnemyCount)
		: m_mapMgr(mapMgr)
		, m_type(type)
		,m_mapMainModel	(nullptr)
		,m_specialModel	(nullptr)
		,m_blockModel	(nullptr)
		,m_baseModel	(nullptr)
		, m_trunalModel(nullptr)
		, m_mirrorModel(nullptr)

		, m_mapEnemyCount(mapEnemyCount)
		, m_nowCanMakeEnemyCount(mapEnemyCount)
	{
		ASELoader aseLoader;
		int	mapData[16][16];

		switch (type)
		{
		case 1:
		case 2:
		case 3:
			m_mapMainModel	= aseLoader.LoadAseFile(L"./Resource/Map/Snowland/World.ASE");
			m_specialModel	= aseLoader.LoadAseFile(L"./Resource/Map/Snowland/Lab.ASE");
			m_blockModel	= aseLoader.LoadAseFile(L"./Resource/Map/Snowland/Block.ASE");	
			m_baseModel		= aseLoader.LoadAseFile(L"./Resource/Map/Snowland/Base.ASE");
			m_trunalModel	= aseLoader.LoadAseFile(L"./Resource/Map/Snowland/Turnal.ASE");
			m_mirrorModel	= aseLoader.LoadAseFile(L"./Resource/Map/Mirror.ASE");

			if (type == 1)
			{
				int tempMapData[16][16] =
				{
					1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
					2,2,2,4,4,0,6,0,6,0,6,0,4,4,1,1,
					2,2,2,4,0,0,0,0,0,0,0,0,4,4,1,1,
					2,2,2,0,0,0,4,4,4,4,4,0,5,4,1,1,
					1,1,8,0,0,0,0,0,0,0,0,0,0,7,1,1,
					1,1,0,4,0,0,0,0,0,0,0,0,4,0,1,1,
					1,1,0,0,0,4,0,0,0,0,4,0,0,0,1,1,
					1,1,5,0,5,4,4,0,0,0,5,5,0,2,2,2,
					1,1,5,0,5,4,4,0,0,0,5,5,0,2,2,2,
					1,1,0,0,0,0,0,0,0,0,0,0,0,2,2,2,
					1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,7,
					1,1,0,0,4,4,5,5,4,4,5,5,0,0,0,0,
					1,1,0,0,4,4,5,5,4,4,5,5,0,0,0,0,
					1,1,0,0,0,0,0,0,9,0,0,0,0,0,0,8,
					1,1,0,0,0,0,0,0,3,0,0,0,2,2,2,2,
					1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,
				};
				memcpy(&mapData, &tempMapData, sizeof(int) * 256);
			}
			else if (type == 2)
			{
				int tempMapData[16][16] =
				{
					1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
					2,2,2,8,6,0,6,0,6,0,6,0,6,7,1,1,
					2,2,2,0,0,0,0,0,0,0,0,0,0,0,1,1,
					2,2,2,0,0,0,0,0,0,0,0,0,0,0,1,1,
					1,1,0,0,0,0,0,8,0,7,0,0,0,0,1,1,
					1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
					1,1,8,0,0,0,0,0,0,0,0,0,0,8,1,1,
					1,1,0,0,0,0,0,0,0,0,0,0,0,2,2,2,
					1,1,0,0,0,0,0,0,0,0,0,0,0,2,2,2,
					1,1,7,0,0,0,0,0,0,0,0,7,0,2,2,2,
					1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					1,1,0,0,0,0,0,0,9,0,0,0,0,0,0,0,
					1,1,4,7,0,0,0,8,3,7,0,8,2,2,2,2,
					1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,
				};
				memcpy(&mapData, &tempMapData, sizeof(int) * 256);
			}
			else
			{
				int tempMapData[16][16] =
				{
					1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
					2,2,2,0,6,0,6,0,6,0,6,0,6,0,1,1,
					2,2,2,0,0,0,0,0,0,0,0,0,0,0,1,1,
					2,2,2,0,5,0,5,0,5,0,5,0,5,0,1,1,
					1,1,0,0,5,5,5,0,5,5,5,0,5,5,1,1,
					1,1,0,5,5,4,5,5,5,4,5,5,5,0,1,1,
					1,1,5,4,5,5,5,4,5,5,5,4,5,5,1,1,
					1,1,0,5,5,4,5,5,5,4,5,5,5,2,2,2,
					1,1,5,4,5,5,5,4,5,5,5,4,5,2,2,2,
					1,1,0,5,5,4,5,5,5,4,5,5,5,2,2,2,
					1,1,5,4,5,5,5,4,5,5,5,4,5,5,0,5,
					1,1,0,5,5,4,5,5,5,4,5,5,5,0,5,5,
					1,1,5,0,5,5,5,0,5,5,5,0,5,5,0,5,
					1,1,5,0,5,0,5,5,9,5,5,0,5,0,0,5,
					1,1,5,0,5,0,5,0,3,0,5,0,2,2,2,2,
					1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,
				};
				memcpy(&mapData, &tempMapData, sizeof(int) * 256);
			}
			break;
		}



		for (int tempY = 0; tempY < 16; tempY++)
		{
			for (int tempX = 0; tempX < 16; tempX++)
			{
				D3DXVECTOR3 tilePosition(MAPTILESIZE * (tempX - 7.5f), 0, MAPTILESIZE * ((15 - tempY) - 7.5f));
				D3DXVECTOR3 preset(0, 0, MAPTILESIZE * -0.2f);
				D3DXVECTOR3 presettedPos = tilePosition + preset;
				switch (mapData[tempY][tempX])
				{
				case 0:
					m_mapTile[tempY][tempX] = nullptr;
					break;
				case 1:
					m_mapTile[tempY][tempX] = new MapTile(m_mapMgr, 1, nullptr, presettedPos);
					break;
				case 2:
					m_mapTile[tempY][tempX] = new MapTile(m_mapMgr, 2, nullptr, presettedPos);
					break;
				case 3:
					m_mapTile[tempY][tempX] = new MapTile(m_mapMgr, 3, m_baseModel, presettedPos);
					break;
				case 4:
					m_mapTile[tempY][tempX] = new MapTile(m_mapMgr, 4, m_specialModel, presettedPos);
					break;
				case 5:
					m_mapTile[tempY][tempX] = new MapTile(m_mapMgr, 5, m_blockModel, presettedPos);
					break;
				case 6:
					m_mapTile[tempY][tempX] = new MapTile(m_mapMgr, 6, m_trunalModel, presettedPos);
					break;

				case 7:
					m_mapTile[tempY][tempX] = new MapTile(m_mapMgr, 7, m_mirrorModel, presettedPos);
					break;
				case 8:
					m_mapTile[tempY][tempX] = new MapTile(m_mapMgr, 7, m_mirrorModel, presettedPos, true);
					break;

				case 9:
					m_mapTile[tempY][tempX] = new MapTile(m_mapMgr, 9, nullptr, presettedPos);
					break;
				default:
					m_mapTile[tempY][tempX] = nullptr;
					break;
				}
			}
		}
	}
	Map::~Map()
	{
		for (int tempY = 0; tempY < 16; tempY++)
			for (int tempX = 0; tempX < 16; tempX++)
				SAFE_DELETE(m_mapTile[tempY][tempX]);
	}



	void Map::Initialize()
	{
		m_nowCanMakeEnemyCount = m_mapEnemyCount;

		for (int tempY = 0; tempY < 16; tempY++)
			for (int tempX = 0; tempX < 16; tempX++)
				if (m_mapTile[tempY][tempX])
					m_mapTile[tempY][tempX]->Initialize();
	}

	void Map::Update()
	{
		if (rand() % 600 == 0)
		{
			while (true)
			{
				int	tempX = rand() % 16;
				int	tempY = rand() % 16;
				if (!(m_mapTile[tempY][tempX] && m_mapTile[tempY][tempX]->IsActive()))
				{
					m_mapMgr->GetItMgr()->CreateItemSender(rand() % 5 + 1, D3DXVECTOR3(MAPTILESIZE * (tempX - 7.5f), 0, MAPTILESIZE * ((15 - tempY) - 7.5f)) + D3DXVECTOR3(0, 0, MAPTILESIZE * -0.2f));
					break;
				}
			}
		}

		for (int tempY = 0; tempY < 16; tempY++)
			for (int tempX = 0; tempX < 16; tempX++)
				if (m_mapTile[tempY][tempX] && m_mapTile[tempY][tempX]->IsActive())
					m_mapTile[tempY][tempX]->Update();
	}

	void Map::Render()
	{
		D3DXMATRIX a;
		D3DXMatrixIdentity(&a);
		m_mapMainModel->SetTransform(a);
		m_mapMainModel->Update();
		m_mapMainModel->Render();
		for (int tempY = 0; tempY < 16; tempY++)
			for (int tempX = 0; tempX < 16; tempX++)
				if (m_mapTile[tempY][tempX] && m_mapTile[tempY][tempX]->IsActive())
					m_mapTile[tempY][tempX]->Render();
	}

	void Map::Release()
	{
	}



	bool Map::OutOfMap(const D3DXVECTOR3 & crashObject, const float crashRange)
	{
		return	 MAPTILESIZE * 8	< crashObject.x + crashRange ||
				-MAPTILESIZE * 8	> crashObject.x - crashRange ||
				 MAPTILESIZE * 8	< crashObject.z + crashRange ||
				-MAPTILESIZE * 8	> crashObject.z - crashRange;
	}

	MapTile * Map::GetCrashTile(const int & type, const D3DXVECTOR3& crashObject, const float crashRange)
	{
		for (int tempY = 0; tempY < 16; tempY++)
			for (int tempX = 0; tempX < 16; tempX++)
				if (m_mapTile[tempY][tempX] && m_mapTile[tempY][tempX]->IsActive() && m_mapTile[tempY][tempX]->GetType() == type)
					if (m_mapTile[tempY][tempX]->CheckCrash(crashObject, crashRange))
						return m_mapTile[tempY][tempX];
		return nullptr;
	}

	void Map::CreatePlayer()
	{
		for (int tempY = 0; tempY < 16; tempY++)
			for (int tempX = 0; tempX < 16; tempX++)
				if (m_mapTile[tempY][tempX] && m_mapTile[tempY][tempX]->GetType() == 9)
					m_mapTile[tempY][tempX]->Update();
	}





	MapTile::MapTile(MapManager* mapMgr, const int& type, ASEModel* tileModel, const D3DXVECTOR3& tile3DWorldPos, const bool& isLookRight)
		:m_mapMgr(mapMgr)
		,m_type(type)
		,m_tileModel(tileModel)
		,m_tile3DWorldPos(tile3DWorldPos)

		,m_isDisabled(true)
		,m_unbreakable(false)
		,m_health(0)

		, m_blockHealth{ 0, 0, 0, 0 }
		, m_crashedBlockIs(0)

		, m_isLookRight(isLookRight)
	{
	}
	MapTile::~MapTile()
	{
	}



	void MapTile::Initialize()
	{
		switch (m_type)
		{
		case 1:
		case 2:
		case 4:
		case 6:
		case 7:
		case 9:
			m_unbreakable = true;
			break;
		case 3:
			m_health = 120;
			break;
		case 5:
			m_health = 1;
			m_blockHealth[0][0] = 20;
			m_blockHealth[0][1] = 20;
			m_blockHealth[1][0] = 20;
			m_blockHealth[1][1] = 20;
			break;
		default:
			break;
		}
		m_isDisabled = false;
	}
	void MapTile::Update()
	{
		switch (m_type)
		{
		case 6:
			if (m_mapMgr->GetMap()->CanIMakeEnemy() && rand() % 900 == 0)
			{
				m_mapMgr->GetEmgr()->CreateEnemy()->SetPosition(m_tile3DWorldPos);
				m_mapMgr->GetMap()->IWillMakeEnemy();
			}
			break;

		case 9:
			m_mapMgr->CreatePlayer(m_tile3DWorldPos);
			SetDestroy();
			break;
		}
	}
	void MapTile::Render()
	{
		if (m_tileModel)
		{
			D3DXMATRIX matT;
			if (m_type == 5)
			{
				for (int tempY = 0; tempY < 2; tempY++)
				{
					for (int tempX = 0; tempX < 2; tempX++)
					{
						if (m_blockHealth[tempY][tempX] > 0)
						{
							D3DXMatrixTranslation(&matT, m_tile3DWorldPos.x + (MAPTILESIZE * (0.25f * (tempX * 2 - 1))), -20, m_tile3DWorldPos.z + (MAPTILESIZE * (0.25f * (1 - tempY * 2))));

							m_tileModel->SetTransform(matT);
							m_tileModel->Update();
							m_tileModel->Render();
						}
					}
				}
			}
			else if (m_type == 7)
			{
				D3DXMATRIX matR;
				D3DXMatrixRotationY(&matR, D3DXToRadian(m_isLookRight ? 45 : -45));
				D3DXMatrixTranslation(&matT, m_tile3DWorldPos.x, m_tile3DWorldPos.y, (m_type == 6 ? m_tile3DWorldPos.z + 15 : m_tile3DWorldPos.z));

				m_tileModel->SetTransform(matR * matT);
				m_tileModel->Update();
				m_tileModel->Render();
			}
			else
			{
				D3DXMatrixTranslation(&matT, m_tile3DWorldPos.x, m_tile3DWorldPos.y, (m_type == 6 ? m_tile3DWorldPos.z + 15 : m_tile3DWorldPos.z));

				m_tileModel->SetTransform(matT);
				m_tileModel->Update();
				m_tileModel->Render();
			}
		}
	}
	void MapTile::Release()
	{
	}

	bool MapTile::CheckCrash(const D3DXVECTOR3& crashObject, const float crashRange)
	{
		if (m_type == 5)
		{
			for (int tempY = 0; tempY < 2; tempY++)
			{
				for (int tempX = 0; tempX < 2; tempX++)
				{
					if (m_blockHealth[tempY][tempX] > 0)
					{
						RECT me, target;
						SetRect(&me, crashObject.x - crashRange * 0.5f, crashObject.z - crashRange * 0.5f, crashObject.x + crashRange * 0.5f, crashObject.z + crashRange * 0.5f);
						SetRect(&target, m_tile3DWorldPos.x - (tempX ? 0 : MAPTILESIZE * 0.5f), m_tile3DWorldPos.z - (tempY ? MAPTILESIZE * 0.5f : 0), m_tile3DWorldPos.x + (tempX ? MAPTILESIZE * 0.5f : 0), m_tile3DWorldPos.z + (tempY ? 0 : MAPTILESIZE * 0.5f));

						if (me.left		< target.right	&&
							me.right	> target.left	&&
							me.top		< target.bottom	&&
							me.bottom	> target.top)
						{
							m_crashedBlockIs = tempX + tempY * 2;
							return true;
						}
					}
				}
			}
		}
		else if (m_type == 7)
		{
			RECT meFirst, meMiddel, meLast, target;
			SetRect(&target, crashObject.x - crashRange * 0.5f, crashObject.z - crashRange * 0.5f, crashObject.x + crashRange * 0.5f, crashObject.z + crashRange * 0.5f);
			SetRect(&meFirst,	m_tile3DWorldPos.x - MAPTILESIZE * 0.5f,		m_tile3DWorldPos.z - MAPTILESIZE * (!m_isLookRight ? -(1.f / 6) : 0.5f),		m_tile3DWorldPos.x - MAPTILESIZE * (1.f / 6),	m_tile3DWorldPos.z + MAPTILESIZE * (!m_isLookRight ? 0.5f : -(1.f / 6)));
			SetRect(&meMiddel,	m_tile3DWorldPos.x - MAPTILESIZE * (1.f / 6),	m_tile3DWorldPos.z - MAPTILESIZE * (1.f / 6),									m_tile3DWorldPos.x + MAPTILESIZE * (1.f / 6),	m_tile3DWorldPos.z + MAPTILESIZE * (1.f / 6));
			SetRect(&meLast,	m_tile3DWorldPos.x + MAPTILESIZE * (1.f / 6),	m_tile3DWorldPos.z + MAPTILESIZE * (!m_isLookRight ? -0.5f : (1.f / 6)),		m_tile3DWorldPos.x + MAPTILESIZE * 0.5f,		m_tile3DWorldPos.z + MAPTILESIZE * (!m_isLookRight ? -(1.f / 6) : 0.5f));
			
			return	(meFirst.left		< target.right	&&
					 meFirst.right		> target.left	&&
					 meFirst.top		< target.bottom	&&
					 meFirst.bottom		> target.top) ||

					(meMiddel.left		< target.right	&&
					 meMiddel.right		> target.left	&&
					 meMiddel.top		< target.bottom	&&
					 meMiddel.bottom	> target.top) ||
					
					(meLast.left		< target.right	&&
					 meLast.right		> target.left	&&
					 meLast.top			< target.bottom	&&
					 meLast.bottom		> target.top);
		}
		else
		{
			RECT me, target;
			SetRect(&me, crashObject.x - crashRange * 0.5f, crashObject.z - crashRange * 0.5f, crashObject.x + crashRange * 0.5f, crashObject.z + crashRange * 0.5f);
			SetRect(&target, m_tile3DWorldPos.x - MAPTILESIZE * 0.5f, m_tile3DWorldPos.z - MAPTILESIZE * 0.5f, m_tile3DWorldPos.x + MAPTILESIZE * 0.5f, m_tile3DWorldPos.z + MAPTILESIZE * 0.5f);

			return	me.left		< target.right	&&
				me.right	> target.left	&&
				me.top		< target.bottom	&&
				me.bottom	> target.top;
		}
		return false;
	}

	void MapTile::CrashDamage(const int & damage)
	{
		if (m_type == 3)
			int a = 5;
		if (m_type == 5)
		{
			(m_crashedBlockIs < 2 ? m_blockHealth[0][m_crashedBlockIs] : m_blockHealth[1][m_crashedBlockIs - 2]) -= damage;
			if (m_blockHealth[0][0] <= 0 &&
				m_blockHealth[0][1] <= 0 &&
				m_blockHealth[1][0] <= 0 &&
				m_blockHealth[1][1] <= 0)
				m_health = 0;
		}
		else
			m_health -= damage;

		if (!m_unbreakable && m_health <= 0)
			SetDestroy();
	}

}
