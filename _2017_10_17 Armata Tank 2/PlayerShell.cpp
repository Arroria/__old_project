#include "stdafx.h"
#include "PlayerShell.h"

#define GameDeltaTime (g_processManager->GetDeltaTime())

PlayerShell::PlayerShell(const D3DXVECTOR3& position, const D3DXVECTOR3& direction, const PlayerShellType& shellType)
	: BaseShell(position, direction)

	, m_txShell(nullptr)
{
	m_collisionSize = D3DXVECTOR2(1, 1);
	switch (shellType)
	{
	case PlayerShellType::Cannon:
		m_txShell = g_textureContainer->GetTexture(L"Shell_Cannon");
		m_firePower = 120;
		m_penetration = 50;
		m_speed = 1.4;
		break;
	case PlayerShellType::AutoCannon:
		m_txShell = g_textureContainer->GetTexture(L"Shell_AutoCannon");
		m_firePower = 10;
		m_penetration = 30;
		m_speed = 2.2;
		break;
	case PlayerShellType::Rocket:
		m_txShell = g_textureContainer->GetTexture(L"Shell_Rocket");
		m_firePower = 200;
		m_penetration = 40;
		m_speed = 0.8;
		break;
	}
	m_speed *= 45;
}
PlayerShell::~PlayerShell()
{
}


void PlayerShell::Move()
{
	m_position += m_direction * m_speed * GameDeltaTime;


	m_rotation = D3DXToDegree(acos(D3DXVec3Dot(&D3DXVECTOR3(0, -1, 0), &m_direction)));
	if (m_direction.x < 0)
		m_rotation = 360 - m_rotation;
}

bool PlayerShell::Render()
{
	D3DXMATRIX t, r, s;
	D3DXMatrixTranslation(&t, m_position.x, m_position.z + 10, m_position.y);
	D3DXMatrixRotationY(&r, -D3DXToRadian(m_rotation));
	D3DXVECTOR3 tpD(1, 1, 1);
	tpD *= 0.6f;
	D3DXMatrixScaling(&s, tpD.x, tpD.y, tpD.z);
	g_ase->GetAse(L"MaybeShell")->Render(s * r * t);

	GetSingleton(TextureRenderer)->SetTransform(m_position, m_rotation, m_scale);
	GetSingleton(TextureRenderer)->DrawTexture(m_txShell);
	return false;
}
