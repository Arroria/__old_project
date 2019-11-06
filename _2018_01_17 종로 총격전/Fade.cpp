#include "stdafx.h"
#include "Fade.h"


Fade::Fade()
	: m_isFadeIn(false)
	, m_time(0)
{
}
Fade::~Fade()
{
}



void Fade::Initialize(const bool& isFadeIn)
{
	m_isFadeIn = isFadeIn;
}

void Fade::Update()
{
	m_time++;
}

#define TileX (16 * 3)
#define TileY (9  * 3)

#define AllTime (60)
#define OffTime (20)
#define RotationPower 360

void Fade::Render(const D3DXVECTOR3 & cameraPosition, D3DXVECTOR3 cameraSize)
{
	cameraSize.z = 0;
	D3DXVECTOR2 size(cameraSize.x * 2 / TileX, cameraSize.y * 2 / TileY);
	for (UINT i = 0; i < TileX; i++)
	{
		float startTime = (1 - (float)i / TileX) * AllTime;
		float result = ((float)m_time - startTime) / OffTime;
		
		if (result < 0)
		{
			if (!m_isFadeIn)
				continue;
			result = 0;
		}
		else if (result > 1)
		{
			if (m_isFadeIn)
				continue;
			result = 1;
		}
	
		D3DXQUATERNION q(0, 0, 0, 1);
		D3DXQuaternionRotationAxis(&q, &D3DXVECTOR3(0, 0, -1), D3DXToRadian(result * RotationPower));
	
		for (UINT j = 0; j < TileY; j++)
		{
			GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"BlackPixel"), cameraPosition - cameraSize + D3DXVECTOR3(((float)i + 0.5f) * size.x, ((float)j + 0.5f) * size.y, 0), false, q, size * (m_isFadeIn ? 1 - result : result));
		}
	}
}

void Fade::Release()
{
}



bool Fade::IsEnd()
{
	return OffTime + AllTime < m_time;
}
