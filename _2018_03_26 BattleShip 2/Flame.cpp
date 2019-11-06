#include "DXUT.h"
#include "Flame.h"


Flame::Flame(const D3DXVECTOR3 & pos)
	: m_pos(pos)
{
	for (UINT i = 0; i < 24; i++)
		TextureLoad(L"./Resource/effect/flame/" + std::to_wstring(i+1) + L".png", L"Flame" + std::to_wstring(i));
}
Flame::~Flame()
{
}



void Flame::Update()
{
	constexpr UINT DELAY = 1;
	constexpr UINT MAX_FRAME = 24;

	m_frame.second++;
	if (m_frame.second >= DELAY)
	{
		m_frame.second = 0;
		m_frame.first++;
		if (m_frame.first >= MAX_FRAME)
		{
			m_frame.first = 0;
			Expire();
			return;
		}
	}
}

void Flame::Render()
{
	Texture* tex = TextureGet(L"Flame" + std::to_wstring(m_frame.first));

	D3DXMATRIX bill; {
		DEVICE->GetTransform(D3DTS_VIEW, &bill);
		D3DXMatrixInverse(&bill, 0, &bill);
		bill._41 = bill._42 = bill._43 = 0;
	}
	D3DXMATRIX sm; {
		constexpr FLOAT SCALE = 1. / 500;
		D3DXMatrixScaling(&sm, SCALE * tex->info.Width, SCALE * tex->info.Height, SCALE);
	}
	D3DXMATRIX tm; {
		D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);
	}

	DEVICE->SetFVF(Plane::GetFVF());
	DEVICE->SetTransform(D3DTS_WORLD, &(sm * bill * tm));
	DEVICE->SetTexture(0, tex->tex);
	DEVICE->SetRenderState(D3DRS_ZENABLE, false);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, false);
	SingletonInstance(Plane)->Render(DEVICE);
	DEVICE->SetRenderState(D3DRS_ZENABLE, true);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

