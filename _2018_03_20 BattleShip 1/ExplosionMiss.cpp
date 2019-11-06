#include "DXUT.h"
#include "ExplosionMiss.h"


ExplosionMiss::ExplosionMiss(const D3DXVECTOR3& pos)
	: m_pos(pos)

	, m_frame(0)
	, m_slowFrame(0)
{
}
ExplosionMiss::~ExplosionMiss()
{
}



void ExplosionMiss::Update()
{
	m_slowFrame++;
	if (m_slowFrame >= 3)
	{
		m_slowFrame = 0;
		m_frame++;
		if (m_frame >= 11)
			Expire();
	}
}

void ExplosionMiss::Render()
{
	Texture* tex = GetTex(L"ExplosionMiss" + std::to_wstring(m_frame));

	D3DXMATRIX invViewM, sm, tm;
	/* Inv View Mat */ {
		DEVICE->GetTransform(D3DTS_VIEW, &invViewM);
		invViewM._41 = invViewM._42 = invViewM._43 = 0;
		D3DXMatrixTranspose(&invViewM, &invViewM);
	}
	constexpr FLOAT EXPLOSION = 10;
	D3DXMatrixScaling(&sm, EXPLOSION, EXPLOSION, EXPLOSION);
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);

	D3DXMATRIX worldM;
	worldM = invViewM * sm* tm;

	DEVICE->SetTransform(D3DTS_WORLD, &worldM);
	DEVICE->SetFVF(Plane::GetFVF());
	DEVICE->SetMaterial(&WHITEMTRL);
	DEVICE->SetTexture(0, tex->tex);
	PlaneRender();
}