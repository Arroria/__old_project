#include "DXUT.h"
#include "ScanEffect.h"


ScanEffect::ScanEffect(const D3DXVECTOR3& pos)
	: m_pos(pos)
	, m_frame(std::make_pair(0, 0))
{
}
ScanEffect::~ScanEffect()
{
}



void ScanEffect::Update()
{
	constexpr UINT DELAY = 4;
	constexpr UINT MAX_FRAME = 10;

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

void ScanEffect::Render()
{
	D3DXMATRIX bill; {
		DEVICE->GetTransform(D3DTS_VIEW, &bill);
		D3DXMatrixInverse(&bill, 0, &bill);
		bill._41 = bill._42 = bill._43 = 0;
	}
	D3DXMATRIX sm; {
		constexpr FLOAT SCALE = 1.5;
		D3DXMatrixScaling(&sm, SCALE, SCALE, SCALE);
	}
	D3DXMATRIX tm; {
		D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);
	}

	DEVICE->SetFVF(Plane::GetFVF());
	DEVICE->SetTexture(0, TextureGet(L"Click_" + std::to_wstring(m_frame.first))->tex);
	DEVICE->SetTransform(D3DTS_WORLD, &(bill * sm * tm));
	DEVICE->SetRenderState(D3DRS_ZENABLE, false);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, false);
	SingletonInstance(Plane)->Render(DEVICE);
	DEVICE->SetRenderState(D3DRS_ZENABLE, true);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
