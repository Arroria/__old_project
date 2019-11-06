#include "stdafx.h"
#include "Stage.h"

#include "PlayerAdmin.h"
#include "ShellAdmin.h"
#include "EnemyTankAdmin.h"

#include "BaseTank.h"
#include "BaseShell.h"
#include "BaseBlock.h"
#include "Map.h"
#include "Item.h"
#include "ItemSupplier.h"

#include "PlayerTank.h"
#include "EnemyTank.h"
#include "DefaultBlock.h"

Stage::Stage()
	: m_playerAdmin(nullptr)
	, m_shellAdmin(nullptr)
	, m_enemyAdmin(nullptr)

	, m_testMap(nullptr)


	, m_cameraRotation(60)
	, m_cameraRange(250)
{
}
Stage::~Stage()
{
}


bool Stage::Activated()
{
	MapData* temp = Map::CreateMapFromFile(L"./Resource/MapData/TestMap.mpd");
	m_testMap = new Map(temp, &m_itemSupplierContainer, &m_itemContainer);
	//delete temp;


	m_playerAdmin = new PlayerAdmin(&m_shellAdmin, &m_enemyAdmin, &m_testMap, &m_itemContainer);
	m_shellAdmin = new ShellAdmin(&m_playerAdmin, &m_enemyAdmin, &m_testMap);
	m_enemyAdmin = new EnemyTankAdmin(&m_playerAdmin, &m_shellAdmin, &m_testMap, &m_itemContainer);

	m_playerAdmin->RegisterPlayer(new PlayerTank(m_testMap->GetPlayerSpawnPoint(), 1000, 50));

	m_enemyAdmin->RegisterEnemyTank(new EnemyTank(m_testMap->GetPositionForTile(3, 3), EnemyTankType::Cannon));
	m_enemyAdmin->RegisterEnemyTank(new EnemyTank(m_testMap->GetPositionForTile(3, 4), EnemyTankType::AutoCannon));
	m_enemyAdmin->RegisterEnemyTank(new EnemyTank(m_testMap->GetPositionForTile(6, 3), EnemyTankType::Rocket));


	m_itemContainer.push_back(new Item(0, m_testMap->GetPositionForTile(1, 1)));
	m_itemContainer.push_back(new Item(0, m_testMap->GetPositionForTile(8, 1)));
	m_itemContainer.push_back(new Item(1, m_testMap->GetPositionForTile(1, 2)));
	m_itemContainer.push_back(new Item(1, m_testMap->GetPositionForTile(8, 2)));
	m_itemContainer.push_back(new Item(2, m_testMap->GetPositionForTile(1, 3)));
	m_itemContainer.push_back(new Item(2, m_testMap->GetPositionForTile(8, 3)));
	m_itemContainer.push_back(new Item(3, m_testMap->GetPositionForTile(1, 4)));
	m_itemContainer.push_back(new Item(3, m_testMap->GetPositionForTile(8, 4)));
	m_itemContainer.push_back(new Item(0, m_testMap->GetPositionForTile(1, 5)));
	m_itemContainer.push_back(new Item(0, m_testMap->GetPositionForTile(8, 5)));

	//D3DXMATRIX mat;
	//D3DXMatrixPerspectiveFovLH(&mat, D3DXToRadian(45), g_processManager->GetWndInfo()->ySize / g_processManager->GetWndInfo()->xSize, 1, 3000);
	//g_processManager->GetDevice()->SetTransform(D3DTS_PROJECTION, &mat);
	//D3DXMatrixLookAtLH(&mat, &D3DXVECTOR3(0, 500, -100), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));
	//g_processManager->GetDevice()->SetTransform(D3DTS_VIEW, &mat);
	
	m_cameraRotation = 60;
	m_cameraRange = 250;
	return false;
}

bool Stage::Update()
{
	if (GetSingleton(InputManager)->IsKeyDown('R'))
	{
		GetSingleton(SceneManager)->ChangeScene(L"StageRestart");
		return false;
	}
	

	if (GetSingleton(InputManager)->IsKeyPressed('Q'))
	{// Q를 누르고 있으면 카메라를 높게 이동
		m_cameraRotation += g_processManager->GetDeltaTime() * 20;
		if (m_cameraRotation > 85)
			m_cameraRotation = 85;
	}
	else if (GetSingleton(InputManager)->IsKeyPressed('E'))
	{// E를 누르고 있으면 카메라를 낮게 이동
		m_cameraRotation -= g_processManager->GetDeltaTime() * 20;
		if (m_cameraRotation < 50)
			m_cameraRotation = 50;
	}

	if (GetSingleton(InputManager)->IsKeyPressed('F'))
	{// F를 누르고 있으면 카메라를 가까이 이동
		m_cameraRange -= g_processManager->GetDeltaTime() * 100;
		if (m_cameraRange < 150)
			m_cameraRange = 150;
	}
	else
	{// F를 누르고 있지 않으면 카메라를 멀리 이동
		m_cameraRange += g_processManager->GetDeltaTime() * 100;
		if (m_cameraRange > 250)
			m_cameraRange = 250;
	}



	//if (GetSingleton(InputManager)->IsKeyDown('Z'))	m_itemContainer.push_back(new Item(0, m_testMap->GetPositionForTile(1, 1)));
	//if (GetSingleton(InputManager)->IsKeyDown('X'))	m_itemContainer.push_back(new Item(1, m_testMap->GetPositionForTile(1, 2)));
	//if (GetSingleton(InputManager)->IsKeyDown('C'))	m_itemContainer.push_back(new Item(2, m_testMap->GetPositionForTile(1, 3)));
	//if (GetSingleton(InputManager)->IsKeyDown('V'))	m_itemContainer.push_back(new Item(3, m_testMap->GetPositionForTile(1, 4)));
	m_testMap->MapUpdate();

	m_playerAdmin->PlayerUpdate();
	m_shellAdmin->ShellUpdate();
	m_enemyAdmin->EnemyUpdate();

	m_shellAdmin->ShellInterAction();

	for (auto& itemIter = m_itemContainer.begin(); itemIter != m_itemContainer.end();)
	{
		if (!*itemIter)
		{
			itemIter = m_itemContainer.erase(itemIter);
			continue;
		}

		(*itemIter)->Update();
		if ((*itemIter)->CheckDestroyThis())
		{
			SAFE_DELETE(*itemIter);
			itemIter = m_itemContainer.erase(itemIter);
		}
		else
			itemIter++;
	}
	for (auto& itemSupplierIter = m_itemSupplierContainer.begin(); itemSupplierIter != m_itemSupplierContainer.end();)
	{
		if (!*itemSupplierIter)
		{
			itemSupplierIter = m_itemSupplierContainer.erase(itemSupplierIter);
			continue;
		}

		(*itemSupplierIter)->Update();
		if ((*itemSupplierIter)->CheckDestroyThis())
		{
			SAFE_DELETE(*itemSupplierIter);
			itemSupplierIter = m_itemSupplierContainer.erase(itemSupplierIter);
		}
		else
			itemSupplierIter++;
	}
	
	//Summon Enemy
	if (m_enemyAdmin->GetEnemyContainer()->size() < 5)
	{
		int x(rand() % 2 ? 1 : 15);
		int y(rand() % 2 ? 1 : 15);
		D3DXVECTOR3& pos = m_testMap->GetPositionForTile(x, y);
		m_enemyAdmin->RegisterEnemyTank(new EnemyTank(pos, (EnemyTankType)(rand() % 3)));
	}

	return false;
}

bool Stage::Render()	
{
	//View 행렬 생성 및 적용
	D3DXMATRIX mat;
	D3DXVECTOR3 eye(m_testMap->GetPositionForTile(8, 8).x, 0, -1 * m_cameraRange);
	D3DXMatrixRotationX(&mat, D3DXToRadian(m_cameraRotation));
	D3DXVec3TransformCoord(&eye, &eye, &mat);

	D3DXMatrixLookAtLH(&mat, &eye, &D3DXVECTOR3(m_testMap->GetPositionForTile(8, 8).x, 0 , m_testMap->GetPositionForTile(8, 8).y), &D3DXVECTOR3(0, 1, 0));
	g_processManager->GetDevice()->SetTransform(D3DTS_VIEW, &mat);





	g_processManager->GetDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	g_processManager->GetDevice()->SetTexture(0, g_textureContainer->GetTexture(L"Ground")->d3dxTexture);
	g_processManager->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	struct temp
	{
		D3DXVECTOR3 p;
		D3DXVECTOR2 uv;
		temp(D3DXVECTOR3 _p, D3DXVECTOR2 _uv) : p(_p), uv(_uv) {}
	};
	std::vector<temp> tmp;
	tmp.push_back(temp(D3DXVECTOR3(-1, 0, 1), D3DXVECTOR2(0, 0)));
	tmp.push_back(temp(D3DXVECTOR3(1, 0, 1), D3DXVECTOR2(1, 0)));
	tmp.push_back(temp(D3DXVECTOR3(1, 0, -1), D3DXVECTOR2(1, 1)));
	tmp.push_back(temp(D3DXVECTOR3(-1, 0, 1), D3DXVECTOR2(0, 0)));
	tmp.push_back(temp(D3DXVECTOR3(1, 0, -1), D3DXVECTOR2(1, 1)));
	tmp.push_back(temp(D3DXVECTOR3(-1, 0, -1), D3DXVECTOR2(0, 1)));

	D3DXMATRIX mat1, mat2;
	D3DXMatrixScaling(&mat2, 102.4 * 5, 1, 102.4 * 5);
	D3DXMatrixTranslation(&mat1, m_testMap->GetPositionForTile(8, 8).x, 0, m_testMap->GetPositionForTile(8, 8).y);
	g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(mat2 * mat1));
	g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &tmp[0], sizeof(temp));



	m_testMap->MapRender();

	m_playerAdmin->PlayerRender();
	m_shellAdmin->ShellRender();
	m_enemyAdmin->EnemyRender();
	for (auto& iter : m_itemContainer)
		if (iter)
			iter->Render();
	for (auto& iter : m_itemSupplierContainer)
		if (iter)
			iter->Render();

	WCHAR buffer[1000];
	SetWindowTextW(g_processManager->GetWndInfo()->hWnd, (g_processManager->GetWndInfo()->name + L" " + _itow(1.f / g_processManager->GetDeltaTime(), buffer, 10)).c_str());
	return false;
}

void Stage::Disabled()
{
	SAFE_DELETE(m_playerAdmin);
	SAFE_DELETE(m_shellAdmin);
	SAFE_DELETE(m_enemyAdmin);

	SAFE_DELETE(m_testMap);

	for (auto& iter : m_itemContainer)
		SAFE_DELETE(iter);
	m_itemContainer.clear();

	for (auto& iter : m_itemSupplierContainer)
		SAFE_DELETE(iter);
	m_itemSupplierContainer.clear();
}
