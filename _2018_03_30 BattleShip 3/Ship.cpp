#include "DXUT.h"
#include "Ship.h"


Ship::Ship(const UINT& size)
	: m_size(size)
	, m_life(size)

	, m_frame(0)
{
}
Ship::~Ship()
{
}



void Ship::SetPlaceData(const POINT& tilePos, const bool & isLookAtY)
{
	m_tilePos = tilePos;
	m_lookAtY = isLookAtY;
}

void Ship::Update(const D3DXVECTOR3& pos)
{
	m_frame++;
	
	
	constexpr FLOAT Y_OFFSET = 0.6;

	float flyingOffset = 0;
	if (m_life)
	{
		constexpr FLOAT FLYING_SPEED = 2.5;
		constexpr FLOAT FLYING_DISTANCE = 0.5;
		flyingOffset = sinf(RADIAN(m_frame * FLYING_SPEED)) * FLYING_DISTANCE;
	}

	m_pos = D3DXVECTOR3(pos.x, pos.y + flyingOffset + Y_OFFSET, pos.z);
}

void Ship::Render()
{
	D3DXMATRIX frameRM;
	D3DXMatrixRotationY(&frameRM, RADIAN(m_lookAtY ? 0 : 90));

	D3DXMATRIX tm;
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);
	DEVICE->SetTransform(D3DTS_WORLD, &(frameRM * tm));


	auto CreateTextureKey = [](const UINT& size, const bool& destroyed)->std::wstring
	{
		std::wstring key = L"ShipX" + std::to_wstring(size);
		return destroyed ? key + L"D" : key;
	};
	std::function<std::wstring(void)> TexKey = std::bind(CreateTextureKey, m_size, !m_life);
	ASEGet(TexKey())->Render();
	if (m_size == 3 && m_life)
	{
		D3DXMATRIX rm;
		m_lookAtY ?
		D3DXMatrixRotationZ(&rm, RADIAN(m_frame * 6)) :
		D3DXMatrixRotationX(&rm, RADIAN(m_frame * 6));

		DEVICE->SetTransform(D3DTS_WORLD, &(frameRM * rm * tm));
		ASEGet(TexKey() + L"Wing")->Render();
	}
}
