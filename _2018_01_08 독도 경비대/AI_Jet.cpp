#include "stdafx.h"
#include "AI_Jet.h"

#include "Jet.h"

AI_Jet::AI_Jet(ASE* model, Jet* player)
	: m_me(nullptr)
	, m_player(player)

	, m_model(model)

	, m_attack(false)
{
}
AI_Jet::~AI_Jet()
{
}



void AI_Jet::Initialize()
{
	m_me = new Jet(m_model);
	m_me->Initialize(nullptr);
}

void AI_Jet::Update()
{
	m_me->Update();
	D3DXVECTOR3 forPlayerDirection(m_player->GetPosition() - m_me->GetPosition());

	if (!m_attack)
	{
		m_attack = D3DXVec3Length(&(m_player->GetPosition() - m_me->GetPosition())) > 200;
		//m_me->Boost(0);
	}
	if (m_attack)
	{
		m_attack = D3DXVec3Length(&(m_player->GetPosition() - m_me->GetPosition())) > 50;
		//m_me->SlowDown(0);

		D3DXVec3Normalize(&forPlayerDirection, &forPlayerDirection);

		//LR ROTATE
		D3DXMATRIX left;
		D3DXVECTOR3 leftV;
		D3DXMatrixRotationAxis(&left, &m_me->GetDirection(), -D3DXToRadian(5));
		D3DXVec3TransformCoord(&leftV, &m_me->GetPitchAxis(), &left);
		D3DXMATRIX right;
		D3DXVECTOR3 rightV;
		D3DXMatrixRotationAxis(&right, &m_me->GetDirection(), D3DXToRadian(5));
		D3DXVec3TransformCoord(&rightV, &m_me->GetPitchAxis(), &right);

		float lrDefaultTheta = D3DXVec3Dot(&forPlayerDirection, &m_me->GetPitchAxis());
		float leftTheta = D3DXVec3Dot(&forPlayerDirection, &leftV);
		float rightTheta = D3DXVec3Dot(&forPlayerDirection, &rightV);

		if (leftTheta < rightTheta)
		{
			if (leftTheta < lrDefaultTheta)
				m_me->RotateLeft(3);
		}
		else
		{
			if (rightTheta < lrDefaultTheta)
				m_me->RotateRight(3);
		}

		//PULL UP DOWN
		D3DXVECTOR3 wingAxis;
		D3DXMATRIX wingMatrix;
		D3DXMatrixRotationQuaternion(&wingMatrix, &m_me->GetRotation());
		D3DXVec3TransformCoord(&wingAxis, &D3DXVECTOR3(1, 0, 0), &wingMatrix);

		D3DXMATRIX up;
		D3DXVECTOR3 upV;
		D3DXMatrixRotationAxis(&up, &wingAxis, D3DXToRadian(5));
		D3DXVec3TransformCoord(&upV, &m_me->GetDirection(), &up);
		D3DXMATRIX down;
		D3DXVECTOR3 downV;
		D3DXMatrixRotationAxis(&down, &wingAxis, -D3DXToRadian(5));
		D3DXVec3TransformCoord(&downV, &m_me->GetDirection(), &down);

		float udDefaultTheta = D3DXVec3Dot(&forPlayerDirection, &m_me->GetDirection());
		float upTheta = D3DXVec3Dot(&forPlayerDirection, &upV);
		float downTheta = D3DXVec3Dot(&forPlayerDirection, &downV);

		if (upTheta < downTheta)
		{
			if (upTheta < udDefaultTheta)
				m_me->PullUp(1);
		}
		else
		{
			if (downTheta < udDefaultTheta)
				m_me->PullDown(1);
		}
	}
}

void AI_Jet::Render()
{
	m_me->Render();
}

void AI_Jet::Release()
{
	m_me->Release();
	SAFE_DELETE(m_me);
}


void AI_Jet::SetPosition(const D3DXVECTOR3 & position) { m_me->SetPosition(position); }
