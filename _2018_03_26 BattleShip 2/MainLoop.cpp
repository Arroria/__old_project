#include "DXUT.h"
#include "MainLoop.h"

#include "Ingame.h"
#include "MainMenu.h"

void MainLoop::Initialize()
{
	//if (AllocConsole())
	{
		freopen("CONIN$", "rb", stdin);
		freopen("CONOUT$", "wb", stdout);
		freopen("CONOUT$", "wb", stderr);
	}
	SingletonInstance(Camera)->Initialize((float)WND_SIZE_X / WND_SIZE_Y, D3DXToRadian(45), 0.001, 1000);

	SingletonInstance(SceneManager)->RegistScene(L"MainMenu", new MainMenu());
	SingletonInstance(SceneManager)->RegistScene(L"Ingame", new Ingame());
	SingletonInstance(SceneManager)->ChangeScene(L"MainMenu");
}

void MainLoop::Update()
{
	SingletonInstance(InputManager)->Begin();
	SingletonInstance(SceneManager)->Update();
	SingletonInstance(InputManager)->End();
}

void MainLoop::Render()
{
	if (!SingletonInstance(SceneManager)->IsSceneChanged())
		DEVICE->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0);
	DEVICE->BeginScene();
	SingletonInstance(Camera)->ApplyDevice(DEVICE);

	SingletonInstance(SceneManager)->Render();

	DEVICE->EndScene();
}

void MainLoop::Release()
{
	SingletonRelease(Plane);
	SingletonRelease(Camera);
	SingletonRelease(SkySphere);
	SingletonRelease(Sprite);
	SingletonRelease(Fader);

	SingletonRelease(ASEManager);
	SingletonRelease(TextureManager);
	SingletonRelease(SceneManager);
	SingletonRelease(InputManager);

	FreeConsole();
}

void MainLoop::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SingletonInstance(InputManager)->MsgProc(msg, wParam, lParam);
}
