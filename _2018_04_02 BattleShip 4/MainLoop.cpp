#include "DXUT.h"
#include "MainLoop.h"

#include "InGame.h"
#include "Title.h"


void MainLoop::Initialize()
{
	//if (AllocConsole())
	{
		freopen("CONIN$", "rb", stdin);
		freopen("CONOUT$", "wb", stdout);
		freopen("CONOUT$", "wb", stderr);
	}

	SingletonInstance(SceneManager)->RegistScene(L"InGame", new InGame());
	SingletonInstance(SceneManager)->RegistScene(L"Title", new Title());
	SingletonInstance(SceneManager)->ChangeScene(L"Title");

	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DEVICE->SetRenderState(D3DRS_SRCBLEND	, D3DBLEND_SRCALPHA);
	DEVICE->SetRenderState(D3DRS_DESTBLEND	, D3DBLEND_INVSRCALPHA);

	SingletonInstance(Camera)->SetPos(D3DXVECTOR3(0, 10, -15));
	SingletonInstance(Camera)->SetAt(D3DXVECTOR3(0, 0, 0));
}

void MainLoop::Update()
{
	SingletonInstance(InputManager)->Begin();
	
	SingletonInstance(SceneManager)->Update();

	SingletonInstance(InputManager)->End();
}

void MainLoop::Render()
{
	SingletonInstance(Camera)->AssignData();

	if (!SingletonInstance(SceneManager)->IsSceneChanged())
		DEVICE->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0);
	DEVICE->BeginScene();
	
	
	SingletonInstance(SceneManager)->Render();
	
	
	DEVICE->EndScene();
}

void MainLoop::Release()
{
	SingletonRelease(TextureManager);
	SingletonRelease(ASEManager);

	SingletonRelease(InputManager);
	SingletonRelease(SceneManager);

	SingletonRelease(ZBufferSetter);
	SingletonRelease(WhiteMtrl);
	SingletonRelease(SkySphere);
	SingletonRelease(BillBoard);
	SingletonRelease(Plane);
	SingletonRelease(Camera);
	SingletonRelease(Sprite);

	FreeConsole();
}

void MainLoop::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SingletonInstance(InputManager)->MsgProc(hWnd, msg, wParam, lParam);
}
