#include "stdafx.h"
#include "Maybe_Gun_Effect.h"


Maybe_Gun_Effect::Maybe_Gun_Effect(const D3DXVECTOR2& position)
	: m_position(position)

	, m_animation(0)
{
}
Maybe_Gun_Effect::~Maybe_Gun_Effect()
{
}


#define AnimationFrameInterval 2

void Maybe_Gun_Effect::Initialize()
{
}

void Maybe_Gun_Effect::Update()
{
	if (m_animation >= AnimationFrameInterval * 11)
		Expire();
	m_animation++;
}

void Maybe_Gun_Effect::Render()
{
	if (m_animation < AnimationFrameInterval * 11)
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(std::wstring(L"FB") + std::to_wstring(m_animation / AnimationFrameInterval)), D3DXVECTOR3(m_position.x, m_position.y, 0),false, D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(0.15, 0.15));
}

void Maybe_Gun_Effect::Release()
{
}
