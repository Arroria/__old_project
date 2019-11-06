#include "DXUT.h"
#include "MissileAttackEffect.h"


MissileAttackEffect::MissileAttackEffect(const D3DXVECTOR3& pos)
	: m_pos(pos)
	, m_frame(std::make_pair(0, 0))
{
}
MissileAttackEffect::~MissileAttackEffect()
{
}



void MissileAttackEffect::Update()
{
	constexpr UINT SPEED = 4;
	constexpr UINT MAX = 12;

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

void MissileAttackEffect::Render()
{
	Texture* tex = TextureGet(L"MissileAttack" + std::to_wstring(m_frame.first));
	constexpr FLOAT SCALE = 6.5 / 860;

	D3DXMATRIX sm, tm;
	D3DXMatrixScaling(&sm, SCALE * tex->info.Width, SCALE * tex->info.Height, SCALE);
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);

	DEVICE->SetTexture(0, tex->tex);
	DEVICE->SetTransform(D3DTS_WORLD, &(sm * BILLBOARD * tm));
	SingletonInstance(Plane)->Render();
}