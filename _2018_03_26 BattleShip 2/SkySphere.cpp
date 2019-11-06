#include "DXUT.h"
#include "SkySphere.h"


SkySphere::SkySphere()
	: m_rotate(0)
{
	ASELoad(L"./Resource/model", L"SkySphere.ase", L"SkySphere");
}
SkySphere::~SkySphere()
{
}



void SkySphere::Update()
{
	constexpr FLOAT ROTATE_SPEED = 0.01;
	m_rotate += ROTATE_SPEED;
}

void SkySphere::Render()
{
	DEVICE->SetRenderState(D3DRS_ZENABLE, false);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, false);

	D3DXMATRIX viewInvM;
	DEVICE->GetTransform(D3DTS_VIEW, &viewInvM);
	D3DXMatrixInverse(&viewInvM, 0, &viewInvM);

	D3DXMATRIX rm, tm;
	D3DXMatrixRotationAxis(&rm, &[](const D3DXVECTOR3& axis)->D3DXVECTOR3
	{
		D3DXVECTOR3 _axis;
		D3DXVec3Normalize(&_axis, &axis);
		return axis;
	}(D3DXVECTOR3(3, 7, 1)), D3DXToRadian(m_rotate));
	D3DXMatrixTranslation(&tm, viewInvM._41, viewInvM._42, viewInvM._43);
	DEVICE->SetTransform(D3DTS_WORLD, &(rm * tm));
	ASEGet(L"SkySphere")->Render();
	
	DEVICE->SetRenderState(D3DRS_ZENABLE, true);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
