#include "MainHeader.h"
#include "Enemy.h"

#include "ASE.h"
#include "EnemyManager.h"
#include "EnemyBulletManager.h"

#define	MoveDirection_UP	0
#define	MoveDirection_DOWN	180
#define	MoveDirection_LEFT	270
#define	MoveDirection_RIGHT	90

#define TankSpeed	3
#define TurnSpeed	5
#define AAAAAA


namespace AF
{
	Enemy::Enemy(AF::EnemyManager* enemyManager, ASEModel* bodyModel, ASEModel* turret)
		: m_enemyManager(enemyManager)
		, m_objectiveMoveDirection(NULL)
		, m_needTure(true)

		, m_chassisModel(bodyModel)
		, m_turretModel(turret)

		, ³­_ÀÌ¹Ì_ÃÑ¾ËÀ»_½ú¾î(0)
		, m_health(1)
		, m_delete(false)

	{
		m_rotation.y = MoveDirection_DOWN;

		D3DXMatrixIdentity(&±ÍÂ÷³¶);

		m_health = 100;
	}
	Enemy::~Enemy()
	{
	}



	void Enemy::Initialize()
	{
		//¾²Áö¸¶
	}

	void Enemy::Update()
	{
		MoveChassis();
		Fire();
	}
	void Enemy::Render()
	{
		m_chassisModel->SetTransform(±ÍÂ÷³¶);
		m_chassisModel->Update();
		m_chassisModel->Render();
		m_turretModel->SetTransform(±ÍÂ÷³¶);
		m_turretModel->Update();
		m_turretModel->Render();
	}
	void Enemy::Release()
	{
		//¾²Áö¸¶
	}



	void Enemy::MoveChassis()
	{
		if (!m_needTure)
			m_needTure = !(rand() % 60);

		if (m_needTure)
		{
			switch (rand() % 4)
			{
				case 0: m_objectiveMoveDirection = MoveDirection_UP;	break;
				case 1: m_objectiveMoveDirection = MoveDirection_DOWN;	break;
				case 2: m_objectiveMoveDirection = MoveDirection_LEFT;	break;
				case 3: m_objectiveMoveDirection = MoveDirection_RIGHT;	break;
			}
			m_needTure = false;
		}

		if (m_rotation.y == m_objectiveMoveDirection)
		{
			D3DXVECTOR3 nextPos = m_position;
				 if (m_objectiveMoveDirection == MoveDirection_UP)		nextPos.z += TankSpeed;
			else if (m_objectiveMoveDirection == MoveDirection_DOWN)	nextPos.z -= TankSpeed;
			else if (m_objectiveMoveDirection == MoveDirection_LEFT)	nextPos.x -= TankSpeed;
			else if (m_objectiveMoveDirection == MoveDirection_RIGHT)	nextPos.x += TankSpeed;

			if (!m_enemyManager->CheckCrashedMap(nextPos))
				m_position = nextPos;
			else
				m_needTure = true;
		}
		else
		{
			//³ªÁß¿¡ ¶÷´Ù·Î °¡µ¶¼ºÀ» ¼öÁ¤ÇØº¼°Í.
			if (bool rotationDirectionClockwise = (m_objectiveMoveDirection <= 180 ? !(m_objectiveMoveDirection < m_rotation.y && m_rotation.y < m_objectiveMoveDirection + 180) : (m_objectiveMoveDirection - 180 < m_rotation.y && m_rotation.y < m_objectiveMoveDirection)))
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
		D3DXMatrixScaling(&matS, m_scale.x * 0.5f, m_scale.y * 0.5f, m_scale.z * 0.5f);
		D3DXMatrixRotationY(&matR, D3DXToRadian(m_rotation.y));
		D3DXMatrixTranslation(&matT, m_position.x, m_position.y + 15, m_position.z);

		±ÍÂ÷³¶ = matS * matR * matT;
	}

	void Enemy::Fire()
	{
		if (!³­_ÀÌ¹Ì_ÃÑ¾ËÀ»_½ú¾î)
		{
			(*m_enemyManager->GetBltMger())->CreateEnemyBullet(m_position, GetDirection());
			³­_ÀÌ¹Ì_ÃÑ¾ËÀ»_½ú¾î = rand() % 180 + 90;
		}
		³­_ÀÌ¹Ì_ÃÑ¾ËÀ»_½ú¾î--;

	}

	void Enemy::GiveDamage(const int & damage)
	{
		if ((m_health -= damage) <= 0)
		{
			m_delete = true;
			if (rand() % 10 == 0)
				m_enemyManager->CreateItem(m_position, rand() % 5 + 1);
		}
	}

}
