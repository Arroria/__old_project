#include "DXUT.h"
#include "SkySphere.h"



SkySphere::SkySphere()
	: m_frame(0)
{
	ASELoad(L"./Resource/model", L"SkySphere.ase",  L"SkySphere");
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
	constexpr FLOAT SCALE = 1;
	constexpr FLOAT ROTATION_SPEED = 1. / 50;
	D3DXMATRIX viewInvM;
	DEVICE->GetTransform(D3DTS_VIEW, &viewInvM);
	D3DXMatrixInverse(&viewInvM, 0, &viewInvM);

	D3DXMATRIX sm, rm, tm;
	D3DXMatrixScaling(&sm, SCALE, SCALE, SCALE);
	D3DXMatrixRotationAxis(&rm, &[](const D3DXVECTOR3& axis)->D3DXVECTOR3
	{
		D3DXVECTOR3 temp;
		return *D3DXVec3Normalize(&temp, &axis);
	}(D3DXVECTOR3(3, 11, 5)), RADIAN(ROTATION_SPEED * m_frame));
	D3DXMatrixTranslation(&tm, viewInvM._41, viewInvM._42, viewInvM._43);

	DEVICE->SetTransform(D3DTS_WORLD, &(sm * rm * tm));
	ZBufferOff();
	ZBufWriteOff();
	ASEGet(L"SkySphere")->Render();
	ZBufferOn();
	ZBufWriteOn();
}