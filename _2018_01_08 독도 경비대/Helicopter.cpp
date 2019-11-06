#include "stdafx.h"
#include "Helicopter.h"

#include "OBBCollision.h"

Helicopter::Helicopter(ASE* bodyModel, ASE* topRotorModel, ASE* tailRotorModel)
	: m_position(0, 0, 0)
	, m_tiltRotation(0, 0, 0, 1)
	, m_objectiveRotation(0, 0, 0, 1)
	, m_identityRotation(0, 0, 0, 1)

	, m_engineOutput(1)
	, m_objectiveEngineOutput(1)

	, m_bodyModel(bodyModel)
	, m_topRotorModel(topRotorModel)
	, m_tailRotorModel(tailRotorModel)

	, m_obb(nullptr)
{
}
Helicopter::~Helicopter()
{
}




void Helicopter::Initialize()
{
	m_obb = new OBBCollision();
	m_obb->Update(&D3DXVECTOR3(0, -10000, 0), &D3DXVECTOR3(2, 1.75, 4.5), nullptr);
}

#define HorizontalRecoverySpeed 0.03f
#define Mobility 0.02f
#define TiltSpeed 2
#define TurnSpeed 1

#define Speed 0.4f

#define EngineOuputRecoverySpeed 0.02f
#define Acceleration 0.02f
#define EnginePower 0.01f

void Helicopter::Update()
{
	D3DXQuaternionSlerp(&m_objectiveRotation, &m_objectiveRotation, &m_identityRotation, HorizontalRecoverySpeed);
	D3DXQuaternionSlerp(&m_tiltRotation, &m_tiltRotation, &m_objectiveRotation, Mobility);

	m_objectiveEngineOutput += (1 - m_objectiveEngineOutput) * EngineOuputRecoverySpeed;
	m_engineOutput += (m_objectiveEngineOutput - m_engineOutput) * Acceleration;

	m_position += (GetPitchAxis() * m_engineOutput + D3DXVECTOR3(0, -1, 0)) * Speed;


	m_obb->Update(&(m_position + D3DXVECTOR3(0, -1.1, -1.1)), nullptr, &m_tiltRotation);
}

void Helicopter::Render()
{
	///m_obb->Render(DEVICE, D3DXCOLOR(1, 1, 1, 1));
	D3DXMATRIX rotationMatrix, translationMatrix;
	D3DXMatrixRotationQuaternion(&rotationMatrix, &m_tiltRotation);
	D3DXMatrixTranslation(&translationMatrix, m_position.x, m_position.y, m_position.z);
	m_bodyModel->Render(rotationMatrix * translationMatrix);
}

void Helicopter::Release()
{
	SAFE_DELETE(m_obb);
}



D3DXVECTOR3 Helicopter::GetAxis(const D3DXVECTOR3& defaultAxis) const
{
	D3DXMATRIX temp;
	D3DXMatrixRotationQuaternion(&temp, &m_tiltRotation);
	D3DXVECTOR3 axis;
	D3DXVec3TransformCoord(&axis, &defaultAxis, &temp);
	return axis;
}

void Helicopter::Rotate(const D3DXVECTOR3 & axis, const float & degree)
{
	D3DXQUATERNION q;
	D3DXQuaternionRotationAxis(&q, &axis, degree);
	m_objectiveRotation *= q;
}


void Helicopter::Turn(const bool & right, const float & power)
{
	Rotate(D3DXVECTOR3(0, 1, 0), D3DXToRadian(power) * (right ? 1 : -1));
	D3DXQUATERNION q;
	D3DXQuaternionRotationAxis(&q, &D3DXVECTOR3(0, 1, 0), D3DXToRadian(power) * (right ? 1 : -1));
	m_identityRotation *= q;
}