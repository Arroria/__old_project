#include "stdafx.h"
#include "Explosion.h"


Explosion::Explosion(const D3DXVECTOR3& pos)
	: m_pos(pos)
	, m_frame(0)
{
}
Explosion::~Explosion()
{
}



void Explosion::Update()
{
	if (m_frame >= 26 - 2 - 1)
		Expire();
	m_frame++;
}

void Explosion::Render()
{
	constexpr FLOAT SCALE = 0.05;

	D3DMATERIAL9 whiteMtrl; ZeroMemory(&whiteMtrl, sizeof(D3DMATERIAL9));
	Texture* texture = GetSingleton(TextureLoader)->LoadTexture(std::wstring(L"./Resource/effect/") + std::to_wstring(m_frame + 2) + L".png", std::wstring(L"Explosion") + std::to_wstring(m_frame));
	DEVICE->SetTexture(0, texture->texture);
	DEVICE->SetMaterial(&whiteMtrl);

	D3DXMATRIX sm, rm, tm;
	D3DXMatrixScaling(&sm, texture->imageInfo.Width * SCALE, texture->imageInfo.Height * SCALE, 1);
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);
	DEVICE->GetTransform(D3DTS_VIEW, &rm);	rm._41 = rm._42 = rm._43 = 0;
	D3DXMatrixTranspose(&rm, &rm);

	DEVICE->SetTransform(D3DTS_WORLD, &(sm * rm * tm));

	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	DEVICE->SetFVF(GetSingleton(Plane)->GetFVF());
	GetSingleton(Plane)->Render(DEVICE);
}
