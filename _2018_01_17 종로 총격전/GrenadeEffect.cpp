#include "stdafx.h"
#include "GrenadeEffect.h"


GrenadeEffect::GrenadeEffect(const D3DXVECTOR2& position)
	: m_position(position)

	, m_animation(0)
{
}
GrenadeEffect::~GrenadeEffect()
{
}


#define AnimationFrameInterval 4

void GrenadeEffect::Initialize()
{
}

void GrenadeEffect::Update()
{
	if (m_animation >= AnimationFrameInterval * 8)
		Expire();
	m_animation++;
}

void GrenadeEffect::Render()
{
	if (m_animation < AnimationFrameInterval * 8)
		GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(std::wstring(L"G") + std::to_wstring(m_animation / AnimationFrameInterval)), D3DXVECTOR3(m_position.x, m_position.y, 0));
}

void GrenadeEffect::Release()
{
}

