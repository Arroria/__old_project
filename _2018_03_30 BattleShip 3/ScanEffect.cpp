#include "DXUT.h"
#include "ScanEffect.h"


ScanEffect::ScanEffect(const D3DXVECTOR3 & pos)
	: m_pos(pos)
	, m_frame(std::make_pair(0, 0))
{
}
ScanEffect::~ScanEffect()
{
}



void ScanEffect::Update()
{
	constexpr UINT ANIME_SPEED = 4;
	constexpr UINT ANIME_MAX = 10;

	m_frame.second++;
	if (m_frame.second >= ANIME_SPEED)
	{
		m_frame.second = 0;
		m_frame.first++;
		if (m_frame.first >= ANIME_MAX)
		{
			m_frame.first = 0;
			Expire();
		}
	}
}

void ScanEffect::Render()
{
	D3DXMATRIX bbM = SingletonInstance(BillBoarder)->GetMatrix();
	D3DXMATRIX sm, tm;
	constexpr FLOAT SCALE = 2;
	D3DXMatrixScaling(&sm, SCALE, SCALE, SCALE);
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);

	SetWhiteMaterial();
	DEVICE->SetTransform(D3DTS_WORLD, &(bbM * sm * tm));
	DEVICE->SetTexture(0, TextureGet(L"Lockoner" + std::to_wstring(m_frame.first))->tex);
	SingletonInstance(Plane)->Render();
}
