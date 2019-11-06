#include "stdafx.h"
#include "Tilte.h"


Tilte::Tilte()
	: m_txMainScreen(g_textureContainer->GetTexture(L"MainScreen"))
{
}
Tilte::~Tilte()
{
}


bool Tilte::Activated()
{
	return false;
}

bool Tilte::Update()
{
	for (int temp = 0; temp < 256; temp++)
	{
		if (GetSingleton(InputManager)->IsKeyDown(temp))
		{
			GetSingleton(SceneManager)->ChangeScene(L"Stage");
			return false;
		}
	}
	return false;
}

bool Tilte::Render()
{
	if (!GetSingleton(TextureRenderer)->Begin())
	{
		D3DXMATRIX mat;
		D3DXMatrixTranslation(&mat, 640, 480, 0);
		GetSingleton(TextureRenderer)->SetTransform(D3DXVECTOR3(960, 540, 0), D3DXVECTOR3(1.4, 1.4, 1.4));
		GetSingleton(TextureRenderer)->DrawTexture(m_txMainScreen);

		GetSingleton(TextureRenderer)->End();
	}
	return false;
}

void Tilte::Disabled()
{
}


