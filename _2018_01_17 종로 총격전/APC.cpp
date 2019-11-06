#include "stdafx.h"
#include "APC.h"

#include "Player.h"
#include "Attack.h"

#define TurretOffset D3DXVECTOR2(0, 88)


APC::APC()
	: m_position(0, 0)
	, m_lookAtRight(true)
	, m_state(0)

	, m_animation(0)

	, m_health(10)

	, m_barrelRotation(0, 0, 0, 1)

	, m_player(nullptr)
	, m_attackContainer(nullptr)
	, m_effectContainer(nullptr)
{
}
APC::~APC()
{
}


void APC::Initialize(std::vector<Attack*>* attackContainer, std::list<Effect*>* effectContainer, const Player * player)
{
	m_attackContainer = attackContainer;
	m_effectContainer = effectContainer;
	m_player = player;

	m_position = D3DXVECTOR2(-1500, 107.5);
	m_aabb.SetPosition(m_position);
	m_aabb.SetSize(D3DXVECTOR2(119.5, 107.5));

	D3DXQuaternionRotationAxis(&m_barrelRotation, &D3DXVECTOR3(0, 0, -1), D3DXToRadian(90));
	m_prevAngle = D3DXToRadian(90);
}

void APC::Update()
{
	switch (m_state)
	{
	case 0:
		if (m_lookAtRight ?
			m_position.x > -600 :
			m_position.x < 600)
		{
			m_position.x = m_lookAtRight ?
				m_position.x = -600 :
				m_position.x = 600;
			m_state = 1;
		}

		m_position.x += m_lookAtRight ? 2 : -2;
		break;

	case 1:
		{
			D3DXVECTOR2 playerPosition = m_player->GetPosition();
			D3DXVECTOR2 turretPivot = m_position + TurretOffset;
			D3DXVECTOR2 direction = playerPosition - turretPivot;
			D3DXVec2Normalize(&direction, &direction);

			m_lookAtRight = D3DXVec2Dot(&direction, &D3DXVECTOR2(1, 0)) >= 0;
			float angle = acos(D3DXVec2Dot(&direction, &D3DXVECTOR2(0, 1)));

			if (angle < D3DXToRadian(90))	angle = D3DXToRadian(90);
			else if (angle > D3DXToRadian(130))	angle = D3DXToRadian(130);
			if (!m_lookAtRight)
				angle = D3DXToRadian(360) - angle;

			CONST float turretRotationSpeed = 0.4f;
				 if (D3DXToRadian(D3DXToDegree(m_prevAngle) + turretRotationSpeed) < angle)
				angle = D3DXToRadian(D3DXToDegree(m_prevAngle) + turretRotationSpeed);
			else if (D3DXToRadian(D3DXToDegree(m_prevAngle) - turretRotationSpeed) > angle)
				angle = D3DXToRadian(D3DXToDegree(m_prevAngle) - turretRotationSpeed);

			D3DXQuaternionRotationAxis(&m_barrelRotation, &D3DXVECTOR3(0, 0, -1), angle);
			D3DXMATRIX tempM;
			D3DXMatrixRotationQuaternion(&tempM, &m_barrelRotation);
			D3DXVECTOR2 turretDirection;
			D3DXVec2TransformCoord(&turretDirection, &D3DXVECTOR2(0, 1), &tempM);

			m_prevAngle = angle;

			if (m_animation % 4 == 0)
			{
				Attack* attack = new Attack();
				attack->Initialize(GetSingleton(TextureLoader)->GetTexture(L"EnemyAttack"), m_position + TurretOffset + turretDirection * 115, m_barrelRotation, 20);
				m_attackContainer->push_back(attack);
			}
		}
		break;

	case 2:
		if (m_lookAtRight ?
			m_position.x < -820 :
			m_position.x > 820)
		{
			if (!m_health)
			{
				m_state = 3;
				g_score += 5000;
				return;
			}

			m_position.x = m_lookAtRight ?
				m_position.x = 820 :
				m_position.x = -820;
			m_state = 0;
			m_lookAtRight = !m_lookAtRight;
			D3DXQuaternionRotationAxis(&m_barrelRotation, &D3DXVECTOR3(0, 0, -1), D3DXToRadian(m_lookAtRight ? 90 : 270));
			m_prevAngle = D3DXToRadian(m_lookAtRight ? 90 : 270);
		}

		m_position.x -= (m_health ? 2 : 0.4) * (m_lookAtRight ? 1 : -1);
		break;

	case 3:
		return;
	}

	m_animation++;
	m_aabb.SetPosition(m_position);
}

void APC::Render()
{
	if (m_state == 3)
		return;

	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"APC_Barrel"), D3DXVECTOR3(m_position.x + TurretOffset.x * (m_lookAtRight ? -1 : 1), m_position.y + TurretOffset.y, 0), m_lookAtRight, m_barrelRotation);
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(std::wstring(L"APC") + std::to_wstring((m_animation / 4) % 2)), D3DXVECTOR3(m_position.x, m_position.y, 0), m_lookAtRight);
}

void APC::Release()
{
}
