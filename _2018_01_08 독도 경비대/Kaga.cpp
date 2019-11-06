#include "stdafx.h"
#include "Kaga.h"

#include "AI_Helicopter.h"
#include "OBBCollision.h"

Kaga::Kaga(std::vector<AI_Helicopter*>* aiSpawnPoint, std::vector<Vulkan*>* enemyAttack, ASE* attackModel)
	: m_model(nullptr)

	, m_position(0, 0, 0)
	, m_rotation(0, 0, 0, 0)

	, m_summonCoolTime(1800)
	, m_attackModel(attackModel)
	, m_enemyAttack(enemyAttack)

	, m_aiSpawnPoint(aiSpawnPoint)

	, m_obb(nullptr)
	, m_health(400)
{
}
Kaga::~Kaga()
{
}



void Kaga::Initialize(Jet* player, ASE* aiModel, ASE* model, const D3DXQUATERNION& data)
{
	m_player = player;
	m_aiModel = aiModel;
	m_model = model;


	D3DXQUATERNION q;
	D3DXQuaternionRotationAxis(&q, &D3DXVECTOR3(0, 1, 0), D3DXToRadian(180));
	m_rotation = data * q;

	D3DXMATRIX r;
	D3DXMatrixRotationQuaternion(&r, &data);
	D3DXVec3TransformCoord(&m_position, &D3DXVECTOR3(0, 0, 1), &r);
	m_position *= 1500;


	m_obb = new OBBCollision();
	m_obb->Update(&D3DXVECTOR3(0, -10000, 0), &D3DXVECTOR3(21, 9, 62), nullptr);
}

void Kaga::Update()
{
	D3DXMATRIX r;
	D3DXMatrixRotationQuaternion(&r, &m_rotation);
	D3DXVECTOR3 direction;
	D3DXVec3TransformCoord(&direction, &D3DXVECTOR3(0, 0, 1), &r);

	m_position += direction * (1. / 12.);

	if (!m_summonCoolTime)
	{
		m_summonCoolTime = 900;
		AI_Helicopter* ai = new AI_Helicopter(m_player, m_enemyAttack, m_attackModel);
		ai->Initialize(m_aiModel, nullptr, nullptr);

		D3DXVECTOR3 aiPosition;
		D3DXVec3TransformCoord(&aiPosition, &D3DXVECTOR3(0, 12, 35), &r);
		ai->SetPosition(m_position + aiPosition);
		ai->SetRotation(m_rotation);
		m_aiSpawnPoint->push_back(ai);
	}
	m_summonCoolTime--;;
	m_obb->Update(&m_position, nullptr, &m_rotation);
}

void Kaga::Render()
{
	D3DXMATRIX r, t;
	D3DXMatrixRotationQuaternion(&r, &m_rotation);
	D3DXMatrixTranslation(&t, m_position.x, m_position.y, m_position.z);
	m_model->Render(r * t);
	///m_obb->Render(DEVICE, D3DXCOLOR(1, .5, .5, 1));
}

void Kaga::Release()
{
	SAFE_DELETE(m_obb);
}
