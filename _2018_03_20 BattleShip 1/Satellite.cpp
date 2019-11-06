#include "DXUT.h"
#include "Satellite.h"


Satellite::Satellite()
	: m_length(50)
	, m_rot1(0)
	, m_rot2(0)
{
}
Satellite::~Satellite()
{
}



void Satellite::Update()
{
	m_rot1 += 1;
}

void Satellite::Render()
{
	DWORD zEnable, zWriteEnable;
	DEVICE->GetRenderState(D3DRS_ZENABLE, &zEnable);
	DEVICE->GetRenderState(D3DRS_ZWRITEENABLE, &zWriteEnable);
	DEVICE->SetRenderState(D3DRS_ZENABLE, true);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);

	constexpr FLOAT SCALE = 1;

	D3DXMATRIX rm;
	D3DXVECTOR3 pos(m_length, 0, 0);
	D3DXMatrixRotationY(&rm, D3DXToRadian(m_rot1));
	D3DXVec3TransformNormal(&pos, &pos, &rm);

	D3DXMATRIX sm, tm;
	D3DXMatrixScaling(&sm, SCALE, SCALE, SCALE);
	D3DXMatrixTranslation(&tm, pos.x, pos.y, pos.z);

	DEVICE->SetTransform(D3DTS_WORLD, &(sm * tm));
	ASEGet(L"Satellite")->Render();

	DEVICE->SetRenderState(D3DRS_ZENABLE, zEnable);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, zWriteEnable);
}
