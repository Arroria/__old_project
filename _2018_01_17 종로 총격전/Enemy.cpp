#include "stdafx.h"
#include "Enemy.h"

#include "Player.h"
#include "Attack.h"


#define GunAccuracy 10
#define GunReboundPower 3
#define GunReboundTime 30

Enemy::Enemy()
	: m_position(0, 0)

	, m_lookAtRight(false)
	, m_gravity(0)
	, m_randomMovingCooltime(180)
	, m_randomMovingDuration(0)
	, m_randomMovingToRight(false)
	, m_courage(rand() % 50)

	, m_walking(0)

	, m_gunRotation(0, 0, 0, 1)
	, m_gunRebound(rand() % GunReboundTime)

	, m_health(3)

	, m_moveType(0)

	, m_player(nullptr)
	, m_attackContainer(nullptr)
{
}
Enemy::~Enemy()
{
}



void Enemy::Initialize(const Player* player, std::vector<Attack*>* attackContainer, const UINT& type)
{
	m_player = player;
	m_attackContainer = attackContainer;

	m_aabb.SetSize(D3DXVECTOR2(20, 20));

	m_moveType = type;
}

void Enemy::Update()
{
	if (!m_health) return;

	switch (m_moveType)
	{
	case 0:	Move1();	break;
	case 1:	Move2();	break;
	case 2:	Move3();	break;
	}
	Aiming();
	_Attack();

	m_aabb.SetPosition(m_position);
}

void Enemy::Render()
{
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(std::wstring(L"Enemy") + (m_walking / 5 ? L"1" : L"0")), D3DXVECTOR3(m_position.x, m_position.y, 0), m_lookAtRight);
	D3DXMATRIX gunRotation;
	D3DXMatrixRotationQuaternion(&gunRotation, &m_gunRotation);
	D3DXVECTOR3 gunPositionOffset;
	D3DXVec3TransformCoord(&gunPositionOffset, &(D3DXVECTOR3(0, -1, 0) * GunReboundPower * m_gunRebound / GunReboundTime), &gunRotation);
	gunPositionOffset.x += (m_lookAtRight ? 1 : -1);
	gunPositionOffset.y -= 2;
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"EnemyGun"), gunPositionOffset + D3DXVECTOR3(m_position.x, m_position.y, 0), m_lookAtRight, m_gunRotation);
	
	//m_aabb.RenderBox(DEVICE);
}

void Enemy::Release()
{
}



void Enemy::Move1()
{
	bool walking = true;

	m_position.y -= m_gravity;
	m_gravity += 0.5f;

	D3DXVECTOR2 intervalVector = m_player->GetPosition() - m_position;
	float intervalLength = D3DXVec2Length(&intervalVector);
	if (intervalLength > 600 - m_courage)
	{
		m_randomMovingCooltime = 60;
		m_randomMovingDuration = 20;
		m_randomMovingToRight = m_lookAtRight;
	}
	else if (intervalLength < 400 - m_courage)
	{
		m_randomMovingCooltime = 180;
		m_randomMovingDuration = 60;
		m_randomMovingToRight = !m_lookAtRight;
	}
	else
		walking = false;

	m_randomMovingCooltime--;
	if (!m_randomMovingCooltime)
	{
		m_randomMovingCooltime = rand() % 60 + 90;
		m_randomMovingDuration = rand() % 15 + 10;
		m_randomMovingToRight = rand() % 2;
	}

	if (m_randomMovingDuration)
	{
		m_randomMovingDuration--;
		m_position.x += m_randomMovingToRight ? 3 : -3;
		walking = true;
	}


	walking ?
		m_walking++ :
		m_walking = 0;
	if (m_walking >= 10)
		m_walking = 0;
}

void Enemy::Move2()
{
	bool walking = true;

	m_position.y -= m_gravity;
	m_gravity += 0.5f;

	D3DXVECTOR2 intervalVector = m_player->GetPosition() - m_position;
	float intervalLength = D3DXVec2Length(&intervalVector);
	if (intervalLength > 500 - m_courage * 0.2f)
	{
		m_randomMovingCooltime = 60;
		m_randomMovingDuration = 20;
		m_randomMovingToRight = m_lookAtRight;
	}
	else if (intervalLength < 150 - m_courage * 0.2f)
	{
		m_randomMovingCooltime = 180;
		m_randomMovingDuration = 60;
		m_randomMovingToRight = !m_lookAtRight;
	}
	else
		walking = false;

	m_randomMovingCooltime--;
	if (!m_randomMovingCooltime)
	{
		m_randomMovingCooltime = rand() % 60 + 90;
		m_randomMovingDuration = rand() % 15 + 10;
		m_randomMovingToRight = rand() % 2;
	}

	if (m_randomMovingDuration)
	{
		m_randomMovingDuration--;
		m_position.x += m_randomMovingToRight ? 2 : -2;
		walking = true;
	}


	walking ?
		m_walking++ :
		m_walking = 0;
	if (m_walking >= 10)
		m_walking = 0;

	if (m_position.x < -700 + 248)
		m_position.x = -700 + 248;
	else if (m_position.x > -700 + 1135)
		m_position.x = -700 + 1135;
}

void Enemy::Move3()
{
	m_position.x = m_position.x < 0 ? -510 : 510;
}

void Enemy::Aiming()
{
	D3DXVECTOR2 playerPosition = m_player->GetPosition();
	D3DXVECTOR2 direction = playerPosition - m_position;
	D3DXVec2Normalize(&direction, &direction);

	m_lookAtRight = D3DXVec2Dot(&direction, &D3DXVECTOR2(1, 0)) >= 0;
	float angle = acos(D3DXVec2Dot(&direction, &D3DXVECTOR2(0, 1)));

		 if (angle < D3DXToRadian(45))	angle = D3DXToRadian(45);
	else if (angle > D3DXToRadian(135))	angle = D3DXToRadian(135);
	if (!m_lookAtRight)
		angle = D3DXToRadian(360) - angle;

	D3DXQuaternionRotationAxis(&m_gunRotation, &D3DXVECTOR3(0, 0, -1), angle);
}

void Enemy::_Attack()
{
	if (m_gunRebound)
		m_gunRebound--;

	if (!m_gunRebound)
	{
		D3DXQUATERNION accuracy(0, 0, 0, 1);
		if (m_moveType != 2)
			D3DXQuaternionRotationAxis(&accuracy, &D3DXVECTOR3(0, 0, 1), D3DXToRadian((rand() % (GunAccuracy * 20)) * 0.1f - GunAccuracy));

		Attack* attack = new Attack();
		attack->Initialize(GetSingleton(TextureLoader)->GetTexture(L"EnemyAttack"), m_position, m_gunRotation * accuracy, 12, rand() % 5);
		m_attackContainer->push_back(attack);

		m_gunRebound = GunReboundTime;
		m_position.x += m_lookAtRight ? -1 : 1;
	}
}
