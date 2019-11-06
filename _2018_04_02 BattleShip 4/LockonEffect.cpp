#include "DXUT.h"
#include "LockonEffect.h"


LockonEffect::LockonEffect(const D3DXVECTOR3& pos)
	: m_pos(pos)
	, m_frame(std::make_pair(0, 0))
{
}
LockonEffect::~LockonEffect()
{
}



void LockonEffect::Update()
{
	constexpr UINT SPEED = 4;
	constexpr UINT MAX = 10;

	m_frame.second++;
	if (m_frame.second >= SPEED)
	{
		m_frame.second = 0;
		m_frame.first++;
		if (m_frame.first >= MAX)
		{
			m_frame.first = 0;
			Expire();
		}
	}
}

void LockonEffect::Render()
{
	constexpr FLOAT SCALE = 1.5;

	D3DXMATRIX sm, tm;
	D3DXMatrixScaling(&sm, SCALE, SCALE, SCALE);
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);

	DEVICE->SetTexture(0, TextureGet(L"Lockon" + std::to_wstring(m_frame.first))->tex);
	DEVICE->SetTransform(D3DTS_WORLD, &(sm * BILLBOARD * tm));
	SingletonInstance(Plane)->Render();
}
