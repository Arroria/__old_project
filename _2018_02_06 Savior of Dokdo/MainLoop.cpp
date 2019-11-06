#include "stdafx.h"
#include "MainLoop.h"

#include "TestScene.h"
#include "Stage1.h"
//TestScene* g_testScene = nullptr;
Stage1* g_testScene = nullptr;

HRESULT MainLoop::Initialize()
{
#if _DEBUG
	if (AllocConsole())
	{
		freopen("CONIN$", "rb", stdin);
		freopen("CONOUT$", "wb", stdout);
		freopen("CONOUT$", "wb", stderr);
	}
#endif

	GetSingleton(CameraManager)->BeginSetting();
	
	//g_testScene = new TestScene();
	g_testScene = new Stage1();
	g_testScene->Initialize();
	
	GetSingleton(CameraManager)->EndSetting(DEVICE);
	GetSingleton(TempShaderHelper)->LoadShader(DEVICE, L"./Resource/fx/NoLight.fx", L"NoLight");
	GetSingleton(TempShaderHelper)->LoadShader(DEVICE, L"./Resource/fx/Diffuse.fx", L"Diffuse");
	GetSingleton(TempShaderHelper)->LoadShader(DEVICE, L"./Resource/fx/LightReflect.fx", L"Light");
	GetSingleton(TempShaderHelper)->LoadShader(DEVICE, L"./Resource/fx/EnemyHealth.fx", L"EnemyHealth");
	return S_OK;
}

void MainLoop::Update(double deltaTime)
{
	GetSingleton(CameraManager)->BeginSetting();
	GetSingleton(InputManager)->Begin();
	
	g_testScene->Update();
	
	GetSingleton(InputManager)->End();
	GetSingleton(CameraManager)->EndSetting(DEVICE);
}

HRESULT MainLoop::Render()
{
	GetSingleton(CameraManager)->SettingApply(DEVICE);

	g_testScene->Render();
	return S_OK;
}

void MainLoop::Release()
{
	SAFE_RELEASE_DELETE(g_testScene);
}

LRESULT MainLoop::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
