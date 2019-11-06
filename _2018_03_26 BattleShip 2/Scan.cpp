#include "DXUT.h"
#include "Scan.h"


Scan::Scan(const D3DXVECTOR3& pos, const POINT & lockonPos, const bool & isPlayerAttack)
	: m_isExpired(false)

	, m_pos(pos)
	, m_lockonPos(lockonPos)
	, m_isPlayerAttack(isPlayerAttack)
	, m_frame(0)
{
	TextureLoad(L"./Resource/map/Scan_0.png", L"Scan0");
	TextureLoad(L"./Resource/map/Scan_1.png", L"Scan1");
}
Scan::~Scan()
{
}



void Scan::Update()
{
	m_frame++;
}

void Scan::Render()
{
	DEVICE->SetFVF(Plane::GetFVF());

	constexpr FLOAT SCALE = 3.2;

	D3DXMATRIX plnRM, sm, rm, tm;
	D3DXMatrixRotationX(&plnRM, D3DXToRadian(90));
	D3DXMatrixScaling(&sm, SCALE, SCALE, SCALE);
	D3DXMatrixRotationY(&rm, D3DXToRadian(m_frame * 8));
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);

	DEVICE->SetTexture(0, TextureGet(L"Scan0")->tex);
	DEVICE->SetTransform(D3DTS_WORLD, &(plnRM * sm * tm));
	SingletonInstance(Plane)->Render(DEVICE);
	
	DEVICE->SetTexture(0, TextureGet(L"Scan1")->tex);
	DEVICE->SetTransform(D3DTS_WORLD, &(plnRM * sm * rm * tm));
	SingletonInstance(Plane)->Render(DEVICE);
}
