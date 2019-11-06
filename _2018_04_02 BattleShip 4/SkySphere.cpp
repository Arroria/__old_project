#include "DXUT.h"
#include "SkySphere.h"


SkySphere::SkySphere()
	: m_frame(0)
{
	ASELoad(L"./Resource/model", L"SkySphere.ase", L"SkySphere");
}
SkySphere::~SkySphere()
{
}



void SkySphere::Update()
{
	m_frame++;
}

void SkySphere::Render()
{
	D3DXMATRIX viewInvM;
	DEVICE->GetTransform(D3DTS_VIEW, &viewInvM);
	D3DXMatrixInverse(&viewInvM, 0, &viewInvM);

	D3DXMATRIX sm, rm, tm;
	D3DXMatrixScaling(&sm, 1, 1, 1);
	D3DXMatrixRotationAxis(&rm, &[](const D3DXVECTOR3& axis)->D3DXVECTOR3
	{
		D3DXVECTOR3 _axis;
		return *D3DXVec3Normalize(&_axis, &axis);
	}(D3DXVECTOR3(3, 11, 5)), D3DXToRadian(m_frame * (1. / 1200) * 6));
	D3DXMatrixTranslation(&tm, viewInvM._41, viewInvM._42, viewInvM._43);

	DEVICE->SetTransform(D3DTS_WORLD, &(sm * rm * tm));
	ASEGet(L"SkySphere")->Render();
}
