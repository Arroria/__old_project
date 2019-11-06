#include "stdafx.h"
#include "Jet.h"

#include "Vulkan.h"

Jet::Jet(ASE* model)
	: m_model(model)

	, m_position(0, 0, 0)
	, m_rotation(0, 0, 0, 1)

	, m_direction(0, 0, 1)
	, m_pitchAxis(0, 1, 0)

	, m_speed(2)
	, m_engineOutput(2)

	, m_attackCooltime(0)

	, m_obb(nullptr)
	, m_health(10)
{
}
Jet::~Jet()
{
}

void Jet::Initialize(std::vector<Vulkan*>* bulletContainer)
{
	m_bulletContainer = bulletContainer;

	m_obb = new OBBCollision();
	m_obb->Update(nullptr, &D3DXVECTOR3(1, 0.25, 1.5), nullptr);
}

void Jet::Update()
{
	//m_engine
	m_speed += (m_engineOutput - m_speed) * 0.05f;
	m_position += m_direction * m_speed * 0.4f;

	//Attack
	if (m_attackCooltime)
		m_attackCooltime--;
	
	//Info
	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationQuaternion(&rotationMatrix, &m_rotation);
	D3DXVec3TransformCoord(&m_direction, &D3DXVECTOR3(0, 0, 1), &rotationMatrix);
	D3DXVec3TransformCoord(&m_pitchAxis, &D3DXVECTOR3(0, 1, 0), &rotationMatrix);


	m_obb->Update(&m_position, nullptr, &m_rotation);
}

void Jet::Render()
{
	D3DXMATRIX translationMatrix;
	D3DXMATRIX rotationMatrix;
	D3DXMatrixTranslation(&translationMatrix, m_position.x, m_position.y, m_position.z);
	D3DXMatrixRotationQuaternion(&rotationMatrix, &m_rotation);
	
	m_model->Render(rotationMatrix * translationMatrix);
	//m_obb->Render(DEVICE, D3DXCOLOR(.5, 1, .5, 1));
}

void Jet::Release()
{
	m_bulletContainer = nullptr;
}



void Jet::JetRotation(const D3DXVECTOR3 & axis, const float & power)
{
	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationQuaternion(&rotationMatrix, &m_rotation);
	D3DXVECTOR3 jetAxis;
	D3DXVec3TransformCoord(&jetAxis, &axis, &rotationMatrix);

	D3DXQUATERNION q;
	D3DXQuaternionRotationAxis(&q, &jetAxis, D3DXToRadian(power));
	//m_objectiveRotation = m_objectiveRotation * q;
	m_rotation = m_rotation * q;
}

void Jet::PullUp(const float& power)		{ JetRotation(D3DXVECTOR3(-1, 0, 0), power); }
void Jet::PullDown(const float& power)		{ JetRotation(D3DXVECTOR3(1, 0, 0), power); }
void Jet::RotateLeft(const float& power)	{ JetRotation(D3DXVECTOR3(0, 0, 1), power); }
void Jet::RotateRight(const float& power)	{ JetRotation(D3DXVECTOR3(0, 0, -1), power); }
void Jet::MoveLeft(const float& power)		{ JetRotation(D3DXVECTOR3(0, -1, 0), power); }
void Jet::MoveRight(const float& power)		{ JetRotation(D3DXVECTOR3(0, 1, 0),	power); }

void Jet::Boost(const float & power)
{
	m_engineOutput += 0.02f;
	if (m_engineOutput > 5)
		m_engineOutput = 5;
}

void Jet::SlowDown(const float & power)
{
	m_engineOutput -= 0.02f;
	if (m_engineOutput < 1)
		m_engineOutput = 1;
}

void Jet::Attack(ASE* bulletASE)
{
	if (m_bulletContainer)
	{
		if (!m_attackCooltime)
		{
			Vulkan* attack = new Vulkan();
			attack->Initialize(bulletASE, m_position, m_rotation, 3);
			m_bulletContainer->push_back(attack);

			m_attackCooltime = 6;
		}
	}
}
