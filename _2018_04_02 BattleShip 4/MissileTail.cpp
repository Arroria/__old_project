#include "DXUT.h"
#include "MissileTail.h"


MissileTail::MissileTail(const D3DXVECTOR3& pos)
	: m_pos(pos)
	, m_frame(std::make_pair(0, 0))
{
}
MissileTail::~MissileTail()
{
}



void MissileTail::Update()
{
	constexpr UINT SPEED = 2;
	constexpr UINT MAX = 9;

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

void MissileTail::Render()
{
	constexpr FLOAT SCALE = 1.5;

	D3DXMATRIX sm, tm;
	D3DXMatrixScaling(&sm, SCALE, SCALE, SCALE);
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);

	DEVICE->SetTexture(0, TextureGet(L"MissileTail" + std::to_wstring(m_frame.first))->tex);
	DEVICE->SetTransform(D3DTS_WORLD, &(sm * BILLBOARD * tm));
	SingletonInstance(Plane)->Render();
}
