#include "DXUT.h"
#include "Lockon.h"



Lockon::Lockon(Ship* launcher, const POINT & lockonPos, const bool & byPlayer, const UINT & stay)
	: m_lockonPos(lockonPos)
	, m_byPlayer(byPlayer)
	, m_launcher(launcher)

	, m_stay(stay)
	, m_frame(std::make_pair(0, 0))
	
	, m_lockonComplate(false)
{
}
Lockon::~Lockon()
{
}



void Lockon::Update()
{
	constexpr UINT ANIME_SPEED = 4;
	constexpr UINT ANIME_MAX = 10;

	if (m_stay)
	{
		m_stay--;
		return;
	}

	m_frame.second++;
	if (m_frame.second >= ANIME_SPEED)
	{
		m_frame.second = 0;
		m_frame.first++;
		if (m_frame.first >= ANIME_MAX)
		{
			m_frame.first = 0;
			m_lockonComplate = true;
		}
	}
}

void Lockon::Render(const D3DXVECTOR3 & pos)
{
	if (m_stay)
		return;

	D3DXMATRIX bbM = SingletonInstance(BillBoarder)->GetMatrix();
	D3DXMATRIX sm, tm;
	constexpr FLOAT SCALE = 2;
	D3DXMatrixScaling(&sm, SCALE, SCALE, SCALE);
	D3DXMatrixTranslation(&tm, pos.x, pos.y, pos.z);

	SetWhiteMaterial();
	DEVICE->SetTransform(D3DTS_WORLD, &(bbM * sm * tm));
	DEVICE->SetTexture(0, TextureGet(L"Lockoner" + std::to_wstring(m_frame.first))->tex);
	SingletonInstance(Plane)->Render();
}

