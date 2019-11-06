#include "stdafx.h"
#include "MainLoop.h"


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
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"OBB", L"./Resource/obbBox.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"BlackPixel", L"./Resource/blackPixel.png");
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"WhitePixel", L"./Resource/whitePixel.png");

	GetSingleton(SceneManager)->Initialize();

	return S_OK;
}

void MainLoop::Update(double deltaTime)
{
	GetSingleton(InputManager)->Begin();
	GetSingleton(SceneManager)->Update();
	GetSingleton(InputManager)->End();
}

HRESULT MainLoop::Render()
{
	GetSingleton(SceneManager)->Render();
	return S_OK;
}

void MainLoop::Release()
{
	ReleaseSingleton(InputManager);
	ReleaseSingleton(PlaneRenderer);
	ReleaseSingleton(TextureLoader);
	GetSingleton(SceneManager)->Release();
	ReleaseSingleton(SceneManager);
#if _DEBUG
	FreeConsole();
#endif
}

LRESULT MainLoop::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	GetSingleton(InputManager)->MsgProc(msg, wParam, lParam);
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
