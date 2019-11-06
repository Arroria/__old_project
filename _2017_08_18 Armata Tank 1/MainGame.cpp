#include "MainHeader.h"
#include "MainGame.h"


#include "InputManager.h"
#include "TextureManager.h"
#include "CameraManager.h"

#include "ASE.h"

#include "ItemManager.h"
#include "PlayerManager.h"
#include "PlayerAttackManager.h"
#include "EnemyManager.h"
#include "EnemyBulletManager.h"
#include "MapManager.h"



MainGame::MainGame()
	: m_inputManager		(new AF::InputManager())
	, m_textureManager		(new AF::TextureManager())

	, m_playerManager		(new AF::PlayerManager		(&m_inputManager, &m_playerAttackManager, &m_enemyManager, &m_enemyBulletManager, &m_mapManager, &m_textureManager, &m_itemManager))
	, m_playerAttackManager	(new AF::PlayerAttackManager(&m_mapManager, &m_enemyManager))
	, m_enemyManager		(new AF::EnemyManager		(&m_mapManager, &m_enemyBulletManager, &m_itemManager))
	, m_enemyBulletManager	(new AF::EnemyBulletManager	(&m_mapManager, &m_playerManager))
	, m_mapManager			(new AF::MapManager			(&m_enemyManager, &m_playerManager, &m_itemManager))
	, m_itemManager			(new AF::ItemManager		(&m_mapManager))
{
}
MainGame::~MainGame()
{
	SAFE_DELETE(m_inputManager);
	SAFE_DELETE(m_textureManager);

	SAFE_DELETE(m_playerManager);
	SAFE_DELETE(m_playerAttackManager);
	SAFE_DELETE(m_enemyManager);
	SAFE_DELETE(m_enemyBulletManager);
	SAFE_DELETE(m_mapManager);
}



static AF::Texture* aimTx;
HRESULT MainGame::Initialize()
{
	D3DLIGHT9	light;
	ZeroMemory(&light, sizeof(light));
	light.Ambient = light.Specular = light.Diffuse = D3DXCOLOR(1, 1, 1, 1);
	light.Direction = D3DXVECTOR3(3, -4, 5);
	light.Type = D3DLIGHT_DIRECTIONAL;

	AF::g_processManager->GetDevice()->SetLight(0, &light);
	AF::g_processManager->GetDevice()->LightEnable(0, true);

	AF::g_processManager->GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	AF::g_processManager->GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	AF::CameraManager cameraManager;
	D3DXVECTOR3 cameraOffSet(0, 0, -90);
	cameraManager.SetViewTransform(D3DXVECTOR3(0, 36,-19) * 22 + cameraOffSet, D3DXVECTOR3(0, 0, 0) + cameraOffSet);
	cameraManager.SetProjectionTransform(45, 100000);

	

	m_textureManager->LoadTexture("Aim", L"./Resource/UI2/aim.png");
	aimTx = m_textureManager->GetTexture("Aim");

	m_mapManager->SetMap(L"Snowland_2");
	m_playerManager->Initialize();

	m_inputManager->Initialize();
	IF_FAILED_RETURN( m_textureManager->Initialize() );
	return S_OK;
}

HRESULT MainGame::Update()
{
	m_inputManager->Update();
	{
		if (m_inputManager->IsKeyDown('Z'))
			m_mapManager->RegenPlayer();
		if (m_inputManager->IsKeyDown('X'))		
			m_playerManager->DestroyPlayer();
		if (m_inputManager->IsKeyPressed('C'))
			m_enemyManager->CreateEnemy();
		if (m_inputManager->IsKeyDown('V'))
			m_mapManager->SetMap(L"Snowland_1");
		if (m_inputManager->IsKeyDown('B'))
			m_mapManager->SetMap(L"Snowland_3");
		if (m_inputManager->IsKeyDown('N'))
			m_mapManager->OffMap();
		if (m_inputManager->IsKeyDown('1'))
			m_itemManager->CreateItemSender(1, D3DXVECTOR3(-200, 0, 0));
		if (m_inputManager->IsKeyDown('2'))
			m_itemManager->CreateItemSender(2, D3DXVECTOR3(-100, 0, 0));
		if (m_inputManager->IsKeyDown('3'))
			m_itemManager->CreateItemSender(3, D3DXVECTOR3(0, 0, 0));
		if (m_inputManager->IsKeyDown('4'))
			m_itemManager->CreateItemSender(4, D3DXVECTOR3(100, 0, 0));
		if (m_inputManager->IsKeyDown('5'))
			m_itemManager->CreateItemSender(5, D3DXVECTOR3(200, 0, 0));

		m_mapManager->Update();	
		m_itemManager->Update();
		m_playerManager->Update();
		m_playerAttackManager->Update();
		m_enemyManager->Update();
		m_enemyBulletManager->Update();
	}

	m_inputManager->Reset();
	return S_OK;
}

HRESULT MainGame::Render()
{
	m_playerManager->Render();
	m_itemManager->Render();
	m_playerAttackManager->Render();
	m_mapManager->Render();
	m_enemyManager->Render();
	m_enemyBulletManager->Render();

	if (SUCCEEDED(m_textureManager->Begin()))
	{
		m_playerManager->RenderUI();

		POINT aimPos = m_inputManager->GetMousePos();
		D3DXMATRIX aimTransformMatrix;
		D3DXMatrixTranslation(&aimTransformMatrix, aimPos.x - aimTx->imageInfo.Width * 0.5f, aimPos.y - aimTx->imageInfo.Height * 0.5f, 0);
		m_textureManager->SetTransform(aimTransformMatrix);
		m_textureManager->TextureRender(aimTx);
		m_textureManager->End();
	}


	return S_OK;
}

HRESULT MainGame::Release()
{
	return S_OK;
}

LRESULT MainGame::WndProc(const HWND & hWnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	LRESULT returnValue = NULL;
	if (m_inputManager && m_inputManager->MsgProc(hWnd, msg, wParam, lParam, returnValue))
		return returnValue;
	return 0;
}
