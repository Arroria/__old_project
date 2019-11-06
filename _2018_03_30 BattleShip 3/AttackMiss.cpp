#include "DXUT.h"
#include "AttackMiss.h"


AttackMiss::AttackMiss(const D3DXVECTOR3& pos)
	: m_pos(pos)

	, m_frame(std::make_pair(0, 0))
{
}
AttackMiss::~AttackMiss()
{
}



void AttackMiss::Update()
{
	constexpr UINT ANIME_SPEED = 2;
	constexpr UINT ANIME_MAX = 24;

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

void AttackMiss::Render()
{
	Texture* tex = TextureGet(L"AttackMiss" + std::to_wstring(m_frame.first));

	D3DXMATRIX bbM = SingletonInstance(BillBoarder)->GetMatrix();
	D3DXMATRIX sm, tm;
	constexpr FLOAT SCALE = 0.006;
	D3DXMatrixScaling(&sm, SCALE * tex->info.Width, SCALE * tex->info.Height, SCALE);
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);

	SetWhiteMaterial();
	DEVICE->SetTransform(D3DTS_WORLD, &(sm * bbM * tm));
	DEVICE->SetTexture(0, tex->tex);
	SingletonInstance(Plane)->Render();
}
