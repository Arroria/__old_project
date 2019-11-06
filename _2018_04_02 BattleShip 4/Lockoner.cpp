#include "DXUT.h"
#include "Lockoner.h"


Lockoner::Lockoner(Ship* launcher, const POINT & lockonPos, const bool & byPlayer, const UINT& stay, const bool & renderOn)
	: m_launcher(launcher)
	, m_lockonPos(lockonPos)
	, m_byPlayer(byPlayer)

	, m_renderOn(renderOn)

	, m_stay(stay)
	, m_frame(std::make_pair( 0, 0 ))
	, m_isReadyToFire(false)
{
}
Lockoner::~Lockoner()
{
}



void Lockoner::Update()
{
	if (m_stay)
		m_stay--;
	else
	{
		constexpr UINT SPEED = 4;
		constexpr UINT MAX = 10;

		m_frame.second++;
		if (m_frame.second >= SPEED)
		{
			m_frame.second = 0;
			m_frame.first++;
			if (m_frame.first >= MAX)
			{
				m_frame.first = 0;
				m_isReadyToFire = true;
			}
		}
	}
}

void Lockoner::Render(const D3DXVECTOR3 & pos)
{
	if (!m_renderOn || m_stay)
		return;

	constexpr FLOAT SCALE = 1.5;

	D3DXMATRIX sm, tm;
	D3DXMatrixScaling(&sm, SCALE, SCALE, SCALE);
	D3DXMatrixTranslation(&tm, pos.x, pos.y, pos.z);

	DEVICE->SetTexture(0, TextureGet(L"Lockon" + std::to_wstring(m_frame.first))->tex);
	DEVICE->SetTransform(D3DTS_WORLD, &(sm * BILLBOARD * tm));
	SingletonInstance(Plane)->Render();
}
