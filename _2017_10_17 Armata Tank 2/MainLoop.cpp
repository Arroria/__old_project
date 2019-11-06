#include "stdafx.h"
#include "MainLoop.h"

#include "Tilte.h"
#include "Stage.h"
#include "StageRestart.h"

#include "Map.h"
bool Initialize()
{
	/// //Create Console
	/// if (AllocConsole())
	/// {
	/// 	freopen("CONIN$", "rb", stdin);
	/// 	freopen("CONOUT$", "wb", stdout);
	/// 	freopen("CONOUT$", "wb", stderr);
	/// }
	
	g_textureContainer = new TextureContainer();

	
	//Texture
	{
		g_textureContainer->LoadTexture(L"MainScreen", L"./Resource/Texture/mainScreen_original.jpg");
		g_textureContainer->LoadTexture(L"Ground", L"./Resource/Texture/Background.png");


		g_textureContainer->LoadTexture(L"Block_Grass", L"./Resource/Texture/grass.png");
		g_textureContainer->LoadTexture(L"Block_Stone", L"./Resource/Texture/stone.png");
		g_textureContainer->LoadTexture(L"Block_Chest", L"./Resource/Texture/chest.png");
		g_textureContainer->LoadTexture(L"Block_Grass_x10", L"./Resource/Texture/grass_x10.png");
		g_textureContainer->LoadTexture(L"Block_Stone_x10", L"./Resource/Texture/stone_x10.png");
		g_textureContainer->LoadTexture(L"Block_Chest_x10", L"./Resource/Texture/chest_x10.png");

		g_textureContainer->LoadTexture(L"BlockBreak0", L"./Resource/Texture/break0.png");
		g_textureContainer->LoadTexture(L"BlockBreak1", L"./Resource/Texture/break1.png");
		g_textureContainer->LoadTexture(L"BlockBreak2", L"./Resource/Texture/break2.png");
		g_textureContainer->LoadTexture(L"BlockBreak3", L"./Resource/Texture/break3.png");
		g_textureContainer->LoadTexture(L"BlockBreak4", L"./Resource/Texture/break4.png");
		g_textureContainer->LoadTexture(L"BlockBreak5", L"./Resource/Texture/break5.png");
		g_textureContainer->LoadTexture(L"BlockBreak6", L"./Resource/Texture/break6.png");
		g_textureContainer->LoadTexture(L"BlockBreak7", L"./Resource/Texture/break7.png");
		g_textureContainer->LoadTexture(L"BlockBreak8", L"./Resource/Texture/break8.png");
		g_textureContainer->LoadTexture(L"BlockBreak9", L"./Resource/Texture/break9.png");
		g_textureContainer->LoadTexture(L"BlockBreak0_x10", L"./Resource/Texture/break0_x10.png");
		g_textureContainer->LoadTexture(L"BlockBreak1_x10", L"./Resource/Texture/break1_x10.png");
		g_textureContainer->LoadTexture(L"BlockBreak2_x10", L"./Resource/Texture/break2_x10.png");
		g_textureContainer->LoadTexture(L"BlockBreak3_x10", L"./Resource/Texture/break3_x10.png");
		g_textureContainer->LoadTexture(L"BlockBreak4_x10", L"./Resource/Texture/break4_x10.png");
		g_textureContainer->LoadTexture(L"BlockBreak5_x10", L"./Resource/Texture/break5_x10.png");
		g_textureContainer->LoadTexture(L"BlockBreak6_x10", L"./Resource/Texture/break6_x10.png");
		g_textureContainer->LoadTexture(L"BlockBreak7_x10", L"./Resource/Texture/break7_x10.png");
		g_textureContainer->LoadTexture(L"BlockBreak8_x10", L"./Resource/Texture/break8_x10.png");
		g_textureContainer->LoadTexture(L"BlockBreak9_x10", L"./Resource/Texture/break9_x10.png");
		
		g_textureContainer->LoadTexture(L"Tank_Wheel",		L"./Resource/Texture/tank_wheel.png");
		g_textureContainer->LoadTexture(L"Tank_Body",		L"./Resource/Texture/tank_body.png");
		g_textureContainer->LoadTexture(L"Tank_Cannon",		L"./Resource/Texture/tank_cannon.png");
		g_textureContainer->LoadTexture(L"Tank_AutoCannon", L"./Resource/Texture/tank_autoCannon.png");
		g_textureContainer->LoadTexture(L"Tank_Rocket",		L"./Resource/Texture/tank_Rocket.png");
		g_textureContainer->LoadTexture(L"Tank_Rader",		L"./Resource/Texture/tank_Rader.png");
		
		g_textureContainer->LoadTexture(L"Shell_Cannon", L"./Resource/Texture/아무튼총알임.png");
		g_textureContainer->LoadTexture(L"Shell_AutoCannon", L"./Resource/Texture/아무튼총알임.png");
		g_textureContainer->LoadTexture(L"Shell_Rocket", L"./Resource/Texture/아무튼총알임.png");

		g_textureContainer->LoadTexture(L"Item_Health",	L"./Resource/Texture/item_health.png");
		g_textureContainer->LoadTexture(L"Item_Cannon", L"./Resource/Texture/item_cannon.png");
		g_textureContainer->LoadTexture(L"Item_AutoCannon", L"./Resource/Texture/item_autoCannon.png");
		g_textureContainer->LoadTexture(L"Item_Rocket", L"./Resource/Texture/item_Rocket.png");
	}
	
	g_ase = new ASELoader();
	g_ase->RegisterAse(L"Item_Health", L"./Resource/Model/item_health.ASE");
	g_ase->RegisterAse(L"Item_Cannon", L"./Resource/Model/item_cannon.ASE");
	g_ase->RegisterAse(L"Item_AutoCannon", L"./Resource/Model/item_autoCannon.ASE");
	g_ase->RegisterAse(L"Item_Rocket", L"./Resource/Model/item_rocket.ASE");
	g_ase->RegisterAse(L"Block_Grass", L"./Resource/Model/block_grass.ASE");
	g_ase->RegisterAse(L"Block_Stone", L"./Resource/Model/block_stone.ASE");
	g_ase->RegisterAse(L"Block_Chest", L"./Resource/Model/block_chest.ASE");

	g_ase->RegisterAse(L"TankTurret",	L"./Resource/Model/tank1t.ASE");
	g_ase->RegisterAse(L"TankBody",		L"./Resource/Model/tank1b.ASE");
	g_ase->RegisterAse(L"Tank_Desert",	L"./Resource/Model/tank2.ASE");
	g_ase->RegisterAse(L"Tank_Forest",	L"./Resource/Model/tank3.ASE");
	g_ase->RegisterAse(L"Tank_Snow",	L"./Resource/Model/tank4.ASE");
	g_ase->RegisterAse(L"ItemSupplier", L"./Resource/Model/itemSupplier.ASE");

	g_ase->RegisterAse(L"MaybeShell", L"./Resource/Model/maybeShell.ASE");
	
	GetSingleton(SceneManager)->RegisterScene(L"MainTitle", new Tilte());
	GetSingleton(SceneManager)->RegisterScene(L"Stage", new Stage());
	GetSingleton(SceneManager)->RegisterScene(L"StageRestart", new StageRestart());
	
	//GetSingleton(SceneManager)->ChangeScene(L"Stage");
	GetSingleton(SceneManager)->ChangeScene(L"MainTitle");
	
	return 0;
}

void Update(double deltaTime)
{
	GetSingleton(InputManager)->Begin();
	GetSingleton(SceneManager)->SceneUpdate();
	GetSingleton(InputManager)->End();
}

bool Render()
{
//
//	struct TEMPC
//	{
//		D3DXVECTOR3 p;
//		D3DCOLOR c;
//
//		TEMPC (D3DXVECTOR3 ps, D3DCOLOR cs) : p(ps), c(cs) {}
//	};
//	std::vector<TEMPC> t;
//
//	for (int i = 0; i <= 2000; i++)
//	{
//		D3DCOLOR c;
//		if (i == 1000)
//			c = D3DXCOLOR(0, 0, 1, 1);
//		else if (i == 0 || i == 2000)
//			c = D3DXCOLOR(0, 0, 1, 1);
//		else if (i % 100 == 0)
//			c = D3DXCOLOR(0.5, 0.5, 1, 1);
//		else if (i % 10 == 0)
//			c = D3DXCOLOR(1, 1, 1, 1);
//		else
//			c = D3DXCOLOR(0.5, 0.5, 0.5, 1);
//
//		t.push_back(TEMPC(D3DXVECTOR3(i - 1000, 0, -1000), c));
//		t.push_back(TEMPC(D3DXVECTOR3(i - 1000, 0, 1000), c));
//	}
//	for (int j = 0; j <= 2000; j++)
//	{
//		D3DCOLOR c;
//		if (j == 1000)
//			c = D3DXCOLOR(1, 0, 0, 1);
//		else if (j == 0 || j == 2000)
//			c = D3DXCOLOR(1, 0, 0, 1);
//		else if (j % 100 == 0)
//			c = D3DXCOLOR(1, 0.5, 0.5, 1);
//		else if (j % 10 == 0)
//			c = D3DXCOLOR(1, 1, 1, 1);
//		else
//			c = D3DXCOLOR(0.5, 0.5, 0.5, 1);
//
//		t.push_back(TEMPC(D3DXVECTOR3(-1000, 0, j-1000), c));
//		t.push_back(TEMPC(D3DXVECTOR3(1000, 0, j-1000), c));
//	}
//	
	{
		D3DXMATRIX mat;
		D3DXMatrixPerspectiveFovLH(&mat, D3DXToRadian(45), g_processManager->GetWndInfo()->xSize / g_processManager->GetWndInfo()->ySize, 1, 3000);
		g_processManager->GetDevice()->SetTransform(D3DTS_PROJECTION, &mat);

	}


	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &mat);
	g_processManager->GetDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	g_processManager->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	//g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST, t.size() / 2, &t[0], sizeof(t));



	GetSingleton(SceneManager)->SceneRender();
	return 0;
}

bool Release()
{
	ReleaseSingleton(SceneManager);
	
	SAFE_DELETE(g_textureContainer);
	ReleaseSingleton(TextureRenderer);
	ReleaseSingleton(InputManager);

	

	/// FreeConsole();
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GetSingleton(InputManager)->WndProc(message, wParam, lParam);
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
