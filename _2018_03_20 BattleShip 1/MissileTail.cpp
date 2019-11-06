#include "DXUT.h"
#include "MissileTail.h"


MissileTail::MissileTail(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir)
	: m_pos(pos)
	, m_dir(dir)

	, m_frame(0)
	, m_slowFrame(0)
{
}
MissileTail::~MissileTail()
{
}



void MissileTail::Update()
{
	m_slowFrame++;
	if (m_slowFrame >= 3)
	{
		m_slowFrame = 0;
		m_frame++;
		//if (m_frame >= 13)
		if (m_frame >= 9)
			Expire();
	}
}

void MissileTail::Render()
{
	Texture* tex = GetTex(L"MissileTail" + std::to_wstring(m_frame));

	D3DXMATRIX invViewM, sm, tm;
	/* Inv View Mat */ {
		DEVICE->GetTransform(D3DTS_VIEW, &invViewM);
		invViewM._41 = invViewM._42 = invViewM._43 = 0;
		D3DXMatrixTranspose(&invViewM, &invViewM);
	}
	constexpr FLOAT EXPLOSION = 1;
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
