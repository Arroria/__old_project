#include "stdafx.h"
#include "SingleTexParticle.h"


SingleTexParticle::SingleTexParticle()
	: m_pos(0, 0, 0)
	, m_rotDegree(0)
	, m_scale(1, 1, 1)

	, m_texture(nullptr)
{
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));
}
SingleTexParticle::~SingleTexParticle()
{
}



void SingleTexParticle::Render()
{
	GetSingleton(TempShaderHelper)->SeleteShader(L"NoLight");
	D3DXVECTOR2 texSize(1, 1);
	if (m_texture)
		texSize = D3DXVECTOR2(m_texture->imageInfo.Width, m_texture->imageInfo.Height);

	D3DXMATRIX sm, rm, tm, billboardM;
	D3DXMatrixScaling(&sm, texSize.x * m_scale.x, texSize.y * m_scale.y, m_scale.z);
	D3DXMatrixRotationZ(&rm, D3DXToRadian(-m_rotDegree));
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);
	DEVICE->GetTransform(D3DTS_VIEW, &billboardM);	billboardM._41 = billboardM._42 = billboardM._43 = 0;
	D3DXMatrixTranspose(&billboardM, &billboardM);

	DEVICE->SetTransform(D3DTS_WORLD, &(sm * rm * billboardM * tm));
	GetSingleton(TempShaderHelper)->SetMaterial("g_material", m_mtrl);
	GetSingleton(TempShaderHelper)->SetTexture("g_texture", m_texture ? m_texture->texture : nullptr);

	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	DEVICE->SetFVF(GetSingleton(Plane)->GetFVF());
	UINT passCount = GetSingleton(TempShaderHelper)->Begin(DEVICE);
	for (UINT i = 0; i < passCount; i++)
	{
		GetSingleton(TempShaderHelper)->BeginDraw(i);
		GetSingleton(Plane)->Render(DEVICE);
		GetSingleton(TempShaderHelper)->EndDraw();
	}
	GetSingleton(TempShaderHelper)->End();
	GetSingleton(TempShaderHelper)->SeleteShader(L"Light");
}
