#include "stdafx.h"
#include "Player.h"

#include "Attack.h"
#include "Grenade.h"

#define GunAccuracy 5
#define GunReboundAngle 30
#define GunReboundTime 20

Player::Player(const Camera& camera, std::vector<Attack*>* attackContainer, std::vector<Grenade*>* grenadeContainer)
	: m_position(0, -150)

	, m_lookAtLeft(true)
	, m_gravity(0)
	, m_isGround(false)
	, m_isDoubleJumped(true)
	, m_autoMovingActivate(false)
	, m_slow(0)

	, m_gunRotation(0, 0, 0, 1)
	, m_gunRebound(std::make_pair(0U, 0U))
	, m_specialAttack(0)
	, m_specialAttackTime(0)
	, m_grenadeCount(0)

	, m_walking(0)

	, m_health(0)

	, m_camera(camera)
	, m_attackContainer(attackContainer)
	, m_grenadeContainer(grenadeContainer)
{
}
Player::~Player()
{
}



void Player::Initialize()
{
	m_aabb.SetSize		(D3DXVECTOR2(21, 38) * 0.5f);
	m_aabb.SetPosition	(m_position);

	m_health = 30;
}

void Player::Update()
{
	Move();
	Aiming();
	_Attack();
}

void Player::Render()
{
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(std::wstring(L"Player") + (m_walking / 5 ? L"1" : L"0")), D3DXVECTOR3(m_position.x, m_position.y, 0), m_lookAtLeft);

	D3DXQUATERNION gun1Rotation, gun2Rotation;
	D3DXQuaternionRotationAxis(&gun1Rotation, &D3DXVECTOR3(0, 0, m_lookAtLeft ? -1 : 1), D3DXToRadian(GunReboundAngle) * m_gunRebound.first / GunReboundTime);
	D3DXQuaternionRotationAxis(&gun2Rotation, &D3DXVECTOR3(0, 0, m_lookAtLeft ? -1 : 1), D3DXToRadian(GunReboundAngle) * m_gunRebound.second / GunReboundTime);
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"Gun"), D3DXVECTOR3(m_position.x, m_position.y, 0) + D3DXVECTOR3(-4, -4, 0), m_lookAtLeft, m_gunRotation * gun1Rotation);
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"Gun"), D3DXVECTOR3(m_position.x, m_position.y, 0) + D3DXVECTOR3(+4, -4, 0), m_lookAtLeft, m_gunRotation * gun2Rotation);
	//m_aabb.RenderBox(DEVICE);


	POINT _temp = GetSingleton(InputManager)->GetMousePos();
	GetSingleton(PlaneRenderer)->SpriteRender(GetSingleton(TextureLoader)->GetTexture(L"Aim"), D3DXVECTOR3(_temp.x, _temp.y, 0));
}

void Player::Release()
{
}



void Player::Move()
{
	bool walking = false;

	m_gravity += 0.5f;
	if (GetSingleton(InputManager)->IsKeyPressed('A') && !m_autoMovingActivate)	{ walking = true; m_position.x += (m_slow ? -1 : -3); }
	if (GetSingleton(InputManager)->IsKeyPressed('D') || m_autoMovingActivate)	{ walking = true; m_position.x += (m_slow ?  1 :  3); }
	if (GetSingleton(InputManager)->IsKeyDown(VK_SPACE))
	{
		if (m_isGround)
			m_gravity = (m_slow ? -5 : -10);
		else if (!m_isDoubleJumped)
		{
			m_gravity = (m_slow ? -5 : -10);
			m_isDoubleJumped = true;
		}
		///m_gravity = -100;
	}
	m_isGround = false;

	m_position.y -= m_gravity;
	m_aabb.SetPosition(D3DXVECTOR2(m_position.x, m_position.y));

	walking ?
		m_walking++ :
		m_walking = 0;
	if (m_walking >= 10)
		m_walking = 0;

	if (m_slow)
		m_slow--;
}

void Player::Aiming()
{
	D3DXVECTOR2 cursor;
	D3DXVECTOR2 windowedPosition;
	D3DXVECTOR2 direction;
	{
		{
			POINT _temp = GetSingleton(InputManager)->GetMousePos();
			cursor.x = _temp.x;
			cursor.y = _temp.y;
		}

		D3DXVECTOR2 v1(m_camera.GetPosition() + m_camera.GetSize());
		D3DXVECTOR2 v2(m_camera.GetPosition() - m_camera.GetSize());

		//Lerp Scale 구하는거임
		auto GetVec1Lerp = [](const float& v1, const float& v2, const float& v)->float { return (v - v1) / (v2 - v1); };
		windowedPosition.x = 1600 -  GetVec1Lerp(v1.x, v2.x, m_position.x) * 1600;
		windowedPosition.y = GetVec1Lerp(v1.y, v2.y, m_position.y) * 900;

		D3DXVec2Normalize(&direction, &(cursor - windowedPosition));
	}

	m_lookAtLeft = D3DXVec2Dot(&direction, &D3DXVECTOR2(1, 0)) < 0;
	float angle = D3DXToRadian(180) - acos(D3DXVec2Dot(&direction, &D3DXVECTOR2(0, 1)));
	if (m_lookAtLeft)
		angle = D3DXToRadian(360) - angle;

	D3DXQuaternionRotationAxis(&m_gunRotation, &D3DXVECTOR3(0, 0, -1), angle);
}

void Player::_Attack()
{
	if (m_gunRebound.first)		m_gunRebound.first--;
	if (m_gunRebound.second)	m_gunRebound.second--;
	if (GetSingleton(InputManager)->IsKeyDown(VK_LBUTTON))
	//if (GetSingleton(InputManager)->IsKeyPressed(VK_LBUTTON))
	{
		if (m_specialAttackTime)
		{
			switch (m_specialAttack)
			{
			case 0:
				for (UINT i = 0; i < 3; i++)
				{
					D3DXQUATERNION accuracy;
					D3DXQuaternionRotationAxis(&accuracy, &D3DXVECTOR3(0, 0, 1), D3DXToRadian((rand() % (GunAccuracy * 20)) * 0.1f - GunAccuracy));

					Attack* attack = new Attack();
					attack->Initialize(GetSingleton(TextureLoader)->GetTexture(L"Attack"), m_position, m_gunRotation * accuracy, 15);
					m_attackContainer->push_back(attack);
				}
				break;

			case 1:
			{
				D3DXQUATERNION accuracy;
				D3DXQuaternionRotationAxis(&accuracy, &D3DXVECTOR3(0, 0, 1), D3DXToRadian((rand() % (GunAccuracy * 20)) * 0.1f - GunAccuracy));

				for (UINT i = 0; i < 3; i++)
				{
					Attack* attack = new Attack();
					attack->Initialize(GetSingleton(TextureLoader)->GetTexture(L"Attack"), m_position, m_gunRotation * accuracy, 15);
					m_attackContainer->push_back(attack);
				}
			}
				break;
			}
		}
		else
		{
			D3DXQUATERNION accuracy;
			D3DXQuaternionRotationAxis(&accuracy, &D3DXVECTOR3(0, 0, 1), D3DXToRadian((rand() % (GunAccuracy * 20)) * 0.1f - GunAccuracy));

			Attack* attack = new Attack();
			attack->Initialize(GetSingleton(TextureLoader)->GetTexture(L"Attack"), m_position, m_gunRotation * accuracy, 15);
			m_attackContainer->push_back(attack);
		}

		(m_gunRebound.first <= m_gunRebound.second ? m_gunRebound.first : m_gunRebound.second) = GunReboundTime;
		m_position.x += (m_lookAtLeft ? 1 : -1);// *15;
	}

	if (GetSingleton(InputManager)->IsKeyDown(VK_RBUTTON) && m_grenadeCount)
	{
		Grenade* granade = new Grenade();
		granade->Initialize(m_position, m_lookAtLeft);
		m_grenadeContainer->push_back(granade);
		m_grenadeCount--;
	}

	if (m_specialAttackTime)
		m_specialAttackTime--;
}
