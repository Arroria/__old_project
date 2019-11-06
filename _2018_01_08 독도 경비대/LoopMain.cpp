#include "stdafx.h"
#include "LoopMain.h"

ASE* skybox = nullptr;

#include "AirWar.h"
AirWar* airWarScene = nullptr;

HRESULT LoopMain::Initialize()
{
	if (AllocConsole())
	{
		freopen("CONIN$", "rb", stdin);
		freopen("CONOUT$", "wb", stdout);
		freopen("CONOUT$", "wb", stderr);
	}

	
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHTTYPE::D3DLIGHT_DIRECTIONAL;
	light.Diffuse = light.Ambient = light.Specular = D3DXCOLOR(0.8, 0.8, 0.8, 1);
	light.Direction = D3DXVECTOR3(3, -4, 5);
	DEVICE->SetLight(0, &light);
	DEVICE->LightEnable(0, true);

	DEVICE->SetRenderState(D3DRS_LIGHTING, true);


	GetSingleton(SceneManager)->Initailize();


	ASELoader temp;
	skybox = temp.LoadASE(L"./Resource/model/skybox", L"skybox.ase");

	return S_OK;
}

void LoopMain::Update(double deltaTime)
{
	GetSingleton(InputManager)->Begin();
	GetSingleton(SceneManager)->Update();
	GetSingleton(InputManager)->End();
}

HRESULT LoopMain::Render()
{
	GetSingleton(SceneManager)->Render();
	return S_OK;
}

void LoopMain::Release()
{
	GetSingleton(SceneManager)->Release();
	SAFE_DELETE(skybox);

	FreeConsole();
}

LRESULT LoopMain::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	GetSingleton(InputManager)->MsgProc(msg, wParam, lParam);

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProcW(hWnd, msg, wParam, lParam);
	}
	return 0;
}
