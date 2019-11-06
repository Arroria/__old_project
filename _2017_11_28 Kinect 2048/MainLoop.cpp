#include "stdafx.h"
#include "MainLoop.h"

#include "InGame.h"
#include "MainTitle.h"

#define WndSizeX g_processManager->GetWndInfo()->xSize
#define WndSizeY g_processManager->GetWndInfo()->ySize

#define BackgroundColor D3DXCOLOR(1, 0.5f, 0.2f, 1)

bool MainLoop::Initialize()
{
	if (AllocConsole())
	{
		#pragma warning(push)
		#pragma warning(disable: 4996)
		_wfreopen(L"CONIN$", L"rb", stdin);
		_wfreopen(L"CONOUT$", L"wb", stdout);
		_wfreopen(L"CONOUT$", L"wb", stderr);
		#pragma warning(pop)
	}

	GetSingleton(SceneManager)->SceneRegister(L"Title", new MainTitle());
	GetSingleton(SceneManager)->SceneRegister(L"InGame", new InGame());

	GetSingleton(SceneManager)->ChangeScene(L"Title");


	g_handInputCalculator = new HandInputCalculator();
	g_kinectManager = new KinectManager();
	g_kinectManager->Initialize();


	return false;
}

void MainLoop::Update(double deltaTime)
{
	g_kinectManager->CalculateHandState();
	g_handInputCalculator->SendHandData(g_kinectManager->GetHandData(g_kinectManager->GetMainBody(), Hand::Right));
	g_handInputCalculator->CalculateHandMoving();
	g_handInputCalculator->CalculateHandClick();


	GetSingleton(InputManager)->Begin();
	GetSingleton(SceneManager)->SceneUpdate();
	GetSingleton(InputManager)->End();
}

bool MainLoop::Render()
{
	D3DXMATRIX mat;
	D3DXMatrixOrthoOffCenterLH(&mat, 0, WndSizeX, WndSizeY, 0, 0, 1);
	g_processManager->GetDevice()->SetTransform(D3DTS_PROJECTION, &mat);


	//Background
	{
		//CreatePlan
		struct TempVertex
		{
			D3DXVECTOR3 pos;
			D3DCOLOR color;
			TempVertex(D3DXVECTOR3 pos, D3DCOLOR color) : pos(pos), color(color) {}
			enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
		};
		std::vector<TempVertex> plane;
		{
			plane.push_back(TempVertex(D3DXVECTOR3(0, 1, 0), BackgroundColor));
			plane.push_back(TempVertex(D3DXVECTOR3(1, 0, 0), BackgroundColor));
			plane.push_back(TempVertex(D3DXVECTOR3(1, 1, 0), BackgroundColor));
			plane.push_back(TempVertex(D3DXVECTOR3(0, 1, 0), BackgroundColor));
			plane.push_back(TempVertex(D3DXVECTOR3(0, 0, 0), BackgroundColor));
			plane.push_back(TempVertex(D3DXVECTOR3(1, 0, 0), BackgroundColor));
		}

		//RenderPlane
		D3DXMATRIX s;
		D3DXMatrixScaling(&s, WndSizeX, WndSizeY, 1);
		g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &s);
		g_processManager->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
		g_processManager->GetDevice()->SetFVF(TempVertex::FVF);
		g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(TempVertex));
	}

	GetSingleton(SceneManager)->SceneRender();
	return false;
}

bool MainLoop::Release()
{
	return false;
}

LRESULT MainLoop::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GetSingleton(InputManager)->WndProc(message, wParam, lParam);

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}
}
