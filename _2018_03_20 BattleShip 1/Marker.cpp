#include "DXUT.h"
#include "Marker.h"


Marker::Marker(const D3DXVECTOR3& pos, const bool& isHit)
	: m_hit(isHit)

	, m_pos(pos)
	, m_angle(0)
{
}
Marker::~Marker()
{
}



void Marker::Update()
{
	constexpr FLOAT ROT_SPEED = 1;
	m_angle += ROT_SPEED;
}

void Marker::Render()
{
	DWORD zEnable, zWriteEnable;
	DEVICE->GetRenderState(D3DRS_ZENABLE, &zEnable);
	DEVICE->GetRenderState(D3DRS_ZWRITEENABLE, &zWriteEnable);
	DEVICE->SetRenderState(D3DRS_ZENABLE, true);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);


	constexpr FLOAT SCALE = 1;

	D3DXMATRIX sm, rm, tm;
	D3DXMatrixScaling(&sm, SCALE, SCALE, SCALE);
	D3DXMatrixRotationY(&rm, D3DXToRadian(m_angle));
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);

	DEVICE->SetTransform(D3DTS_WORLD, &(sm * rm * tm));
	ASEGet(std::wstring(L"marker_") + (m_hit ? L"true" : L"fail"))->Render();

	DEVICE->SetRenderState(D3DRS_ZENABLE, zEnable);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, zWriteEnable);
}
