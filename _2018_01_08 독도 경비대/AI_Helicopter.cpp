#include "stdafx.h"
#include "AI_Helicopter.h"

#include "Helicopter.h"
#include "Jet.h"
#include "Vulkan.h"

AI_Helicopter::AI_Helicopter(Jet* temp, std::vector<Vulkan*>* enemyAttack, ASE* attackModel)
	: m_me(nullptr)
	
	, m_temp(temp)

	, m_tempHealth(10)
	
	, m_attackModel(attackModel)
	, m_attackCooltime(0)
	, m_enemyAttack(enemyAttack)

	, m_delete(false)
{
}
AI_Helicopter::~AI_Helicopter()
{
}



void AI_Helicopter::Initialize(ASE* bodyModel, ASE* topRotorModel, ASE* tailRotorModel)
{
	m_me = new Helicopter(bodyModel, topRotorModel, tailRotorModel);
	m_me->Initialize();
}

void AI_Helicopter::Update()
{
	if (m_tempHealth <= 0)
		m_delete = true;

	D3DXVECTOR3 myPosition(m_me->GetPosition());
		 if (myPosition.y < 20)		m_me->Engine(0.02f);
	else if (myPosition.y > 400)	m_me->Engine(-0.02f);
	
	D3DXVECTOR3 directionForPlayer = m_temp->GetPosition() - myPosition;
	D3DXVec3Normalize(&directionForPlayer, &directionForPlayer);
	{
		D3DXVECTOR3 leftV, defaultV, rightV, temp;
		D3DXMATRIX leftRot, rightRot;
		D3DXMatrixRotationAxis(&leftRot, &m_me->GetPitchAxis(), D3DXToRadian(-3));
		D3DXMatrixRotationAxis(&rightRot, &m_me->GetPitchAxis(), D3DXToRadian(3));

		defaultV = m_me->GetRollAxis();
		D3DXVec3TransformCoord(&leftV, &defaultV, &leftRot);
		D3DXVec3TransformCoord(&rightV, &defaultV, &rightRot);
		D3DXVec3Normalize(&leftV, &leftV);
		D3DXVec3Normalize(&defaultV, &defaultV);
		D3DXVec3Normalize(&rightV, &rightV);

		float leftDot	= D3DXVec3Dot(&directionForPlayer, &leftV),
			defaultDot	= D3DXVec3Dot(&directionForPlayer, &defaultV),
			rightDot	= D3DXVec3Dot(&directionForPlayer, &rightV);
		
		bool rightLargeThanLeft = (rightDot > leftDot);
		if ((rightLargeThanLeft ? rightDot : leftDot) > defaultDot)
		{
			m_me->Tilt(rightLargeThanLeft, 2);
			m_me->Turn(rightLargeThanLeft, 1);
		}


		float rangeForTarget = D3DXVec3Length(&(m_temp->GetPosition() - myPosition));
			 if (rangeForTarget < 100)	{ m_me->Pull(true, 1); }
		else if (rangeForTarget > 300)	{ m_me->Pull(false, 1); }

		float yDifference = m_temp->GetPosition().y - myPosition.y;
			 if (yDifference > 10)	m_me->Engine(0.01f);
		else if (yDifference < -10)	m_me->Engine(-0.01f);
	}


	if (!m_attackCooltime)
	{
		m_attackCooltime = 8;
		
		if (D3DXVec3Length(&(m_temp->GetPosition() - myPosition)) < 300)
		{
			Vulkan* vulkan = new Vulkan();
			vulkan->Initialize(m_attackModel, m_me->GetPosition(), m_me->GetRotation(), 2.5f);
			m_enemyAttack->push_back(vulkan);
		}
	}

	m_attackCooltime--;
	m_me->Update();
}

void AI_Helicopter::Render()
{
	m_me->Render();
}

void AI_Helicopter::Release()
{
	m_me->Release();
	SAFE_DELETE(m_me);
}



void AI_Helicopter::SetPosition(const D3DXVECTOR3 & position) { m_me->SetPosition(position); }
void AI_Helicopter::SetRotation(const D3DXQUATERNION & rotation) { m_me->SetRotation(rotation); }
OBBCollision & AI_Helicopter::GetOBB()	{ return m_me->GetOBB(); }
