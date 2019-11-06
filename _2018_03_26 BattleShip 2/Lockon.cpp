#include "DXUT.h"
#include "Lockon.h"


Lockon::Lockon(const POINT & lockonPos, const bool& isPlayerAttack, const UINT & stay)
	: m_isExpired(false)

	, m_lockonPos(lockonPos)
	, m_frame(std::make_pair(0, 0))
	, m_stay(stay)
	, m_isPlayerAttack(isPlayerAttack)
{
	m_sound = nullptr;
}
Lockon::~Lockon()
{
}



void Lockon::Update()
{
	if (m_stay)
		m_stay--;
	else
	{
		if (!m_sound)
			m_sound = SingletonInstance(SoundManager)->Play(L"Lockon");


		constexpr UINT DELAY = 4;
		constexpr UINT MAX_FRAME = 10;

		m_frame.second++;
		if (m_frame.second >= DELAY)
		{
			m_frame.second = 0;
			m_frame.first++;
			if (m_frame.first >= MAX_FRAME)
			{
				m_frame.first = 0;
				Expire();
				return;
			}
		}
	}
}

Texture* Lockon::GetTexture()
{
	return TextureGet(L"Click_" + std::to_wstring(m_frame.first));
}
