#include "DXUT.h"
#include "Scanner.h"


Scanner::Scanner(const D3DXVECTOR3 & pos, const POINT & lockonPos, const bool & byPlayer)
	: m_pos(pos)
	, m_lockonPos(lockonPos)
	, m_byPlayer(byPlayer)

	, m_frame(0)
{
}
Scanner::~Scanner()
{
}




void Scanner::Update()
{
	m_frame++;
}

void Scanner::Render()
{
	D3DXMATRIX sm, rm, tm;
	constexpr FLOAT SCALE = 3;
	D3DXMatrixScaling(&sm, SCALE, SCALE, SCALE);
	D3DXMatrixRotationX(&rm, RADIAN( 90 ));
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);

	SetWhiteMaterial();
	DEVICE->SetTransform(D3DTS_WORLD, &(rm * sm * tm));
	DEVICE->SetTexture(0, TextureGet(L"Scanner0")->tex);
	SingletonInstance(Plane)->Render();

	D3DXMATRIX rrm;
	tm._42 += 0.01;
	D3DXMatrixRotationY(&rrm, -RADIAN( m_frame * 6 * 1 ));
	DEVICE->SetTransform(D3DTS_WORLD, &(rm * sm * rrm * tm));
	DEVICE->SetTexture(0, TextureGet(L"Scanner1")->tex);
	SingletonInstance(Plane)->Render();
}



bool Scanner::IsEnd()
{
	return m_frame > 180;
}
