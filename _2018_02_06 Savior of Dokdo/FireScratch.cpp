#include "stdafx.h"
#include "FireScratch.h"


FireScratch::FireScratch(const D3DXVECTOR3& pos, const D3DXCOLOR& color)
	: m_dir(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50)
	, m_speed(rand() % 100 / 50.f)
	, m_duraction(rand() % 20 + 20)
	, m_flowTime(0)
{
	D3DXVec3Normalize(&m_dir, &m_dir);

	m_pos = pos;
	m_mtrl.Ambient = m_mtrl.Diffuse = m_mtrl.Specular = color;
}
FireScratch::~FireScratch()
{
}



void FireScratch::Update()
{
	if (m_flowTime >= m_duraction - 1)
	{
		Expire();
		return;
	}

	m_flowTime++;
	m_pos += m_dir * m_speed;
	m_scale.x = m_scale.y = 1 - ((float)m_flowTime / m_duraction);
}
