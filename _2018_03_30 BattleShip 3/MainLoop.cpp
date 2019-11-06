#include "DXUT.h"
#include "MainLoop.h"

#include "InGame.h"

void MainLoop::Initialize()
{
	if (AllocConsole())
	{
		freopen("CONIN$", "rb", stdin);
		freopen("CONOUT$", "wb", stdout);
		freopen("CONOUT$", "wb", stderr);
	}

	SingletonInstance(Camera)->SetView(RADIAN(45), (float)WND_SIZE_X / WND_SIZE_Y, 1. / 1024, 128);
	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	SingletonInstance(SceneManager)->RegistScene(L"InGame", new InGame());

	SingletonInstance(SceneManager)->SceneChange(L"InGame");
}

void MainLoop::Update()
{
	SingletonInstance(InputManager)->Begin();


	SingletonInstance(Camera)->SetPos(D3DXVECTOR3(0, 25, -25));
	SingletonInstance(Camera)->SetAt(D3DXVECTOR3(0, 0, 0));

	SingletonInstance(SceneManager)->Update();


	SingletonInstance(InputManager)->End();
}

void MainLoop::Render()
{
	SingletonInstance(Camera)->Assign();
	SingletonInstance(BillBoarder)->Update();

	if (!SingletonInstance(SceneManager)->IsSceneChanged())
		DEVICE->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0);
	DEVICE->BeginScene();

	SingletonInstance(SceneManager)->Render();

	DEVICE->EndScene();
}

void MainLoop::Release()
{
	SingletonRelease(Plane);
	SingletonRelease(Camera);
	SingletonRelease(WhiteMaterial);
	SingletonRelease(SkySphere);
	SingletonRelease(BillBoarder);

	SingletonRelease(TextureManager);
	SingletonRelease(ASEManager);

	SingletonRelease(InputManager);
	SingletonRelease(SceneManager);

	FreeConsole();
}

void MainLoop::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SingletonInstance(InputManager)->MsgProc(msg, wParam, lParam);
}
