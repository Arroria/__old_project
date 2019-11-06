#include "DXUT.h"
#include "Scanner.h"


Scanner::Scanner(const POINT& scanningPos, const bool& byPlayer)
	: m_scanningPos(scanningPos)
	, m_byPlayer(byPlayer)

	, m_frame(0)

	, m_isUsed(false)
{
}
Scanner::~Scanner()
{
}



bool Scanner::Complate() { return m_frame > 120; }
bool Scanner::IsEnd() { return m_frame > 180; }



void Scanner::Update()
{
	m_frame++;
}

void Scanner::Render(const D3DXVECTOR3& pos)
{
	constexpr FLOAT SCALE = 3.5;

	D3DXMATRIX sm, rm, tm;
	D3DXMatrixScaling(&sm, SCALE, SCALE, SCALE);
	D3DXMatrixRotationX(&rm, D3DXToRadian(90));
	D3DXMatrixTranslation(&tm, pos.x, pos.y + 1, pos.z);

	DEVICE->SetTexture(0, TextureGet(L"Scanner0")->tex);
	DEVICE->SetTransform(D3DTS_WORLD, &(sm * rm * tm));
	SingletonInstance(Plane)->Render();

	D3DXMATRIX rrm;
	D3DXMatrixRotationY(&rrm, D3DXToRadian(m_frame * 6));

	DEVICE->SetTexture(0, TextureGet(L"Scanner1")->tex);
	DEVICE->SetTransform(D3DTS_WORLD, &(sm * rm * rrm * tm));
	SingletonInstance(Plane)->Render();
}
