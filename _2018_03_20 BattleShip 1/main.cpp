#include "DXUT.h"
#include "main.h"


#include "Intro.h"
#include "MainMenu.h"
#include "InGame.h"

void MainLoop::Initialize()
{
	/* Console */ {
		if (0&&AllocConsole())
		{
			freopen("CONIN$", "rb", stdin);
			freopen("CONOUT$", "wb", stdout);
			freopen("CONOUT$", "wb", stderr);
		}
	}

	/* DEVICE Render State */ {
		DEVICE->SetRenderState(D3DRS_LIGHTING, true);
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(light));
		light.Diffuse = light.Ambient  = light.Specular = D3DXCOLOR(1, 1, 1, 1);
		light.Direction = -D3DXVECTOR3(3, 4, 5);
		light.Type = D3DLIGHT_DIRECTIONAL;
		DEVICE->SetLight(0, &light);
		DEVICE->LightEnable(0, true);

		DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	SingletonInstance(Fader)->Initialize();


	SingletonInstance(SceneManager)->RegisterScene(new Intro(), L"Intro");
	SingletonInstance(SceneManager)->RegisterScene(new InGame(), L"InGame");
	SingletonInstance(SceneManager)->RegisterScene(new MainMenu(), L"MainMenu");


	SingletonInstance(SoundManager)->LoadSound(L"./Resource/snd/Destroy.wav"			, L"Hit"			, 8);
	SingletonInstance(SoundManager)->LoadSound(L"./Resource/snd/Destroy_Failed.wav"		, L"Miss"			, 8);
	SingletonInstance(SoundManager)->LoadSound(L"./Resource/snd/Ingame.wav"				, L"BGM_InGame"		, 1);
	SingletonInstance(SoundManager)->LoadSound(L"./Resource/snd/Main.wav"				, L"BGM_Main"		, 1);
	SingletonInstance(SoundManager)->LoadSound(L"./Resource/snd/MainButtenSelect.wav"	, L"Btn"			, 8);
	SingletonInstance(SoundManager)->LoadSound(L"./Resource/snd/Select.wav"				, L"SeleteShip"		, 8);
	SingletonInstance(SoundManager)->LoadSound(L"./Resource/snd/Tile_Select.wav"		, L"SeleteTile"		, 16);
	SingletonInstance(SoundManager)->LoadSound(L"./Resource/snd/Shoot.wav"				, L"Missile"		, 8);
	SingletonInstance(SoundManager)->LoadSound(L"./Resource/snd/Win.wav"				, L"Victory"		, 1);


	SingletonInstance(SceneManager)->ChangeScene(L"Intro");
}

void MainLoop::Update()
{
	SingletonInstance(InputManager)->Begin();

	SingletonInstance(SceneManager)->Update();
	/* Cam */ {

		static float angleY = 0;
		static float angleX = 30;
		if (SingletonInstance(InputManager)->IsKeyPressed(VK_MBUTTON))
		{
			POINT mouseDelta = SingletonInstance(InputManager)->GetMouseDelta();
			angleX += mouseDelta.y;
			angleY += mouseDelta.x;

			if (angleX < 30)	angleX = 30;
			else if (angleX >= 90)	angleX = 90 - 0.0001;
		}
		D3DXMATRIX rotX, rotY;
		D3DXMatrixRotationX(&rotX, D3DXToRadian(angleX));
		D3DXMatrixRotationY(&rotY, D3DXToRadian(angleY));

		D3DXVECTOR3 v;
		D3DXVec3TransformNormal(&v, &D3DXVECTOR3(0, 0, -1), &(rotX * rotY));


		CAMERA->SetPos(v * 30);
		CAMERA->SetAt(D3DXVECTOR3(0, 0, 0));
	}

	SingletonInstance(InputManager)->End();
}

void MainLoop::Render()
{
	SingletonInstance(Camera)->SetMatrix();
	SingletonInstance(SceneManager)->Render();
}

void MainLoop::Release()
{
	SingletonRelease(Skybox);
	SingletonRelease(Camera);
	SingletonRelease(Plane);
	SingletonRelease(Sprite);
	SingletonRelease(WhiteMtrl);

	SingletonRelease(SceneManager);
	SingletonRelease(ASEManager);
	SingletonRelease(TextureManager);
	SingletonRelease(InputManager);
	SingletonRelease(SoundManager);
	/* Console */ FreeConsole();
}

LRESULT MainLoop::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SingletonInstance(InputManager)->MsgProc(hWnd, msg, wParam, lParam);
	return 0;
}
