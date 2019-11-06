#include "stdafx.h"
#include "Insergent.h"
#include "Attack.h"


Insergent::Insergent()
	: m_position(0, -100)

	, m_attackPattern((rand() % 8) + 3)
	, m_attackCount(0)
	, m_wait(0)

	, m_health(0)
	, m_stun(0)

	, m_attackContainer(nullptr)
{
}
Insergent::~Insergent()
{
}

#define AttackDelay 4


void Insergent::Initialize(std::vector<Attack*>* attackContainer)
{
	m_attackContainer = attackContainer;

	m_aabb.SetSize(D3DXVECTOR2(135, 64));
	m_health = 3;
}

void Insergent::Update(const D3DXVECTOR2 & cameraPosition, const D3DXVECTOR2 & cameraSize)
{
	if (m_stun)
	{
		m_stun--;
		if (!m_stun)
			m_health = 3;
		return;
	}

	if (m_position.x < cameraPosition.x - cameraSize.x - 100)
	{
		m_attackCount = 0;
		m_attackPattern = (rand() % 8) + 3;

		m_position.x = cameraPosition.x + cameraSize.x + 100;
	}
	else
	{
		m_position.x -= 4;
		
		if (!(m_attackCount % AttackDelay))
		{
			if (!((m_attackCount / AttackDelay) % m_attackPattern))
				m_wait = 2;

			if (m_wait)
				m_wait--;
			else
			{
				Attack* attack = new Attack();
				attack->Initialize(GetSingleton(TextureLoader)->GetTexture(L"InsergentAttack"), m_position + D3DXVECTOR2(15, 98), D3DXQUATERNION(0, 0, 0, 1), 8);
				m_attackContainer->push_back(attack);
			}
		}
		m_attackCount++;
	}
	m_aabb.SetPosition(m_position - D3DXVECTOR2(0, 24));

	m_animation++;
}

void Insergent::Render()
{
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(std::wstring(L"Insergent") + std::to_wstring((m_animation / 4) % 4)), D3DXVECTOR3(m_position.x, m_position.y, 0));
}

void Insergent::Release()
{
}
