#include "MainHeader.h"
#include "Player.h"
#include "ASE.h"

#include "InputManager.h"
#include "PlayerManager.h"
#include "PlayerAttackManager.h"
#include "PlayerBullet.h"
#include "ItemManager.h"
#include "Item.h"

#define	MoveDirection_UP	0
#define	MoveDirection_DOWN	180
#define	MoveDirection_LEFT	270
#define	MoveDirection_RIGHT	90

#define TankSpeed	8
#define TurnSpeed	6
#define AAAAAA


namespace AF
{
	Player::Player(AF::PlayerManager* playerManager, Player3DModel playerModel, PlayerInfo playerInfo)
		: m_playerManager(playerManager)

		, m_playerModel(playerModel)
		, m_playerInfo(playerInfo)

		, m_objectiveMoveDirection(MoveDirection_UP)
		, m_turretDirection(m_objectiveMoveDirection)

		, m_health(100)
		, m_shield(0)
	{
		m_rotation.y = MoveDirection_UP;

		D3DXMatrixIdentity(&m_chassisTransformMatrix);
		D3DXMatrixIdentity(&m_TurretTransformMatrix);
	}
	Player::~Player()
	{
	}



	void Player::Initialize()
	{
		//쓰지마
	}

	void Player::Update()
	{
		MoveChassis();
		RotationTurret();
		Fire();

		Item* item = nullptr;
		if (item = (*m_playerManager->GetItemManager())->CheckGetItem(m_position, 22))
		{
			switch (item->GetItemType())
			{
			case 1:														break;
			case 2:														break;
			case 3:	m_health += 30;	if (m_health > 100) m_health = 100;	break;
			case 4:	m_shield += 3;	if (m_shield > 3)	m_shield = 3;	break;
			case 5:														break;
			}
			item->SetDelete();
		}
	}
	void Player::Render()
	{
		m_playerModel.chassis	->SetTransform(m_chassisTransformMatrix);
		m_playerModel.chassis	->Update();
		m_playerModel.chassis	->Render();
		m_playerModel.turret	->SetTransform(m_TurretTransformMatrix);
		m_playerModel.turret	->Update();
		m_playerModel.turret	->Render();
		if (m_playerModel.gun)
		{
			m_playerModel.gun	->SetTransform(m_TurretTransformMatrix);
			m_playerModel.gun	->Update();
			m_playerModel.gun	->Render();
		}
	}
	void Player::Release()
	{
		//쓰지마
	}



	void Player::MoveChassis()
	{
		InputManager* inputManager = *m_playerManager->GetInputManager();

			 if (inputManager->IsKeyPressed('W') || inputManager->IsKeyPressed(VK_UP))		m_objectiveMoveDirection = MoveDirection_UP		;
		else if (inputManager->IsKeyPressed('S') || inputManager->IsKeyPressed(VK_DOWN))	m_objectiveMoveDirection = MoveDirection_DOWN	;
		else if (inputManager->IsKeyPressed('A') || inputManager->IsKeyPressed(VK_LEFT))	m_objectiveMoveDirection = MoveDirection_LEFT	;
		else if (inputManager->IsKeyPressed('D') || inputManager->IsKeyPressed(VK_RIGHT))	m_objectiveMoveDirection = MoveDirection_RIGHT	;
		
		if (m_rotation.y == m_objectiveMoveDirection)
		{
			D3DXVECTOR3 nextPos = m_position;
				 if (inputManager->IsKeyPressed('W') || inputManager->IsKeyPressed(VK_UP))		nextPos.z	+= TankSpeed;
			else if (inputManager->IsKeyPressed('S') || inputManager->IsKeyPressed(VK_DOWN))	nextPos.z	-= TankSpeed;
			else if (inputManager->IsKeyPressed('A') || inputManager->IsKeyPressed(VK_LEFT))	nextPos.x	-= TankSpeed;
			else if (inputManager->IsKeyPressed('D') || inputManager->IsKeyPressed(VK_RIGHT))	nextPos.x	+= TankSpeed;

			if (!m_playerManager->CheckCrashedMap(nextPos))
				m_position = nextPos;
		}
		else
		{
			//나중에 람다로 가독성을 수정해볼것.
			if (bool rotationDirectionClockwise = ( m_objectiveMoveDirection <= 180 ? !(m_objectiveMoveDirection < m_rotation.y && m_rotation.y < m_objectiveMoveDirection + 180) : (m_objectiveMoveDirection - 180 < m_rotation.y && m_rotation.y < m_objectiveMoveDirection)))
			{
				m_rotation.y += TurnSpeed;
				if (m_rotation.y >= 360)
					m_rotation.y -= 360;

				if (m_rotation.y > m_objectiveMoveDirection && m_rotation.y < m_objectiveMoveDirection + 180)
					m_rotation.y = m_objectiveMoveDirection;
			}
			else
			{
				m_rotation.y -= TurnSpeed;
				if (m_rotation.y < 0)
					m_rotation.y += 360;

				if (m_rotation.y < m_objectiveMoveDirection && m_rotation.y > m_objectiveMoveDirection - 180)
					m_rotation.y = m_objectiveMoveDirection;
			}
		}
		m_position.y = -30;

		D3DXMATRIX matS, matR, matT;
		D3DXMatrixScaling(&matS, m_scale.x, m_scale.y, m_scale.z);
		D3DXMatrixRotationY(&matR, D3DXToRadian( m_rotation.y + 90 ));
		D3DXMatrixTranslation(&matT, m_position.x, m_position.y, m_position.z);

		m_chassisTransformMatrix = matS * matR * matT;
	}

	void Player::RotationTurret()
	{
		POINT mousePos = (*m_playerManager->GetInputManager())->GetMousePos();
		RECT clientRect;
		GetClientRect(AF::g_processManager->GetWindowInfo().hWnd, &clientRect);
		
		D3DXVECTOR3 direction;
		D3DXVec3Normalize(&direction, &D3DXVECTOR3((mousePos.x - (clientRect.right * 0.5f)), mousePos.y - (clientRect.bottom * 0.5f), 0));

		float cosAngle = acos(D3DXVec3Dot(&direction, &D3DXVECTOR3(1, 0, 0)));;
		float directionAngle = (D3DXToDegree(cosAngle) - 90) * -1;
		if (direction.y > 0)
		{
			if (directionAngle > 0)
				directionAngle += 2 * (90 - directionAngle);
			else
				directionAngle += 2 * (-90 - directionAngle);
		}
		m_direction.x = direction.x;
		m_direction.y = 0;
		m_direction.z = -direction.y;


		D3DXMATRIX matS, matR, matT;
		D3DXMatrixScaling(&matS, m_scale.x, m_scale.y, m_scale.z);
		D3DXMatrixRotationY(&matR, D3DXToRadian(directionAngle + 90));
		D3DXMatrixTranslation(&matT, m_position.x, m_position.y, m_position.z);

		m_TurretTransformMatrix = matS * matR * matT;
	}

	void Player::Fire()
	{
		//if ((*m_playerManager->GetInputManager())->IsKeyDown(VK_LBUTTON))
		if ((*m_playerManager->GetInputManager())->IsKeyPressed(VK_LBUTTON))
			(*m_playerManager->GetAttackManager())->CreatePlayerBullet(BulletType_Default, m_position, m_direction);
	}

	void Player::GiveDamage(const int & damage)
	{
		if (m_shield)
		{
			m_shield--;
			return;
		}

		m_health -= damage;
		if (m_health <= 0)
			m_playerManager->DestroyPlayer();
	}

}