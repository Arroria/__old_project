#include "stdafx.h"
#include "Jet.h"


Jet::Jet()
	: m_objectiveRollRotation(0, 0, 0, 1)
	, m_objectiveYawRotation(0, 0, 0, 1)

	, m_objectiveSpeed(1)
{
}
Jet::~Jet()
{
}



void Jet::Initialize()
{
}

void Jet::Update(const INT& accelerate, const INT& rollToRight)
{
	Roll(rollToRight);
	Accelerate(accelerate);
}

void Jet::Render(LPDIRECT3DDEVICE9 device, ASE* model)
{
	D3DXMATRIX rm, tm;
	D3DXMatrixRotationQuaternion(&rm, &(m_rollRotation * m_yawRotation));
	D3DXMatrixTranslation(&tm, m_position.x, m_position.y, m_position.z);

	model->SetFVF(device);
	UINT attributeCount = model->GetAttributeCount();
	for (UINT attribute = 0; attribute < attributeCount; attribute++)
	{
		GeomObj* geomObj = model->GetGeomObj(attribute);
		GetSingleton(TempShaderHelper)->SetMaterial("g_material", geomObj->mtrlTex->first);
		GetSingleton(TempShaderHelper)->SetTexture("g_texture",   geomObj->mtrlTex->second);
		DEVICE->SetTransform(D3DTS_WORLD, &( geomObj->localMatrixSR * rm * geomObj->localMatrixT * tm ));
		
		UINT passCount = GetSingleton(TempShaderHelper)->Begin(device);
		for (UINT i = 0; i < passCount; i++)
		{
			GetSingleton(TempShaderHelper)->BeginDraw(i);
			model->Render(device, attribute);
			GetSingleton(TempShaderHelper)->EndDraw();
		}
		GetSingleton(TempShaderHelper)->End();
	}
}

void Jet::Release()
{
}



void Jet::Roll(const INT& rollToRight)
{
	D3DXQUATERNION objectiveRollRotation, objectiveYawRotation;
	D3DXQuaternionRotationAxis(&objectiveRollRotation, &D3DXVECTOR3(0, 0, -1), D3DXToRadian(rollToRight * 35));
	D3DXQuaternionSlerp(&m_objectiveRollRotation, &m_objectiveRollRotation, &objectiveRollRotation, 0.2f);
	D3DXQuaternionSlerp(&m_rollRotation, &m_rollRotation, &m_objectiveRollRotation, 0.1f);

	if (rollToRight)
	{
		D3DXQuaternionRotationAxis(&objectiveYawRotation, &D3DXVECTOR3(0, 1, 0), D3DXToRadian(rollToRight * 0.75f));
		m_objectiveYawRotation *= objectiveYawRotation;
	}
	D3DXQuaternionSlerp(&m_yawRotation, &m_yawRotation, &m_objectiveYawRotation, 0.1f);
}

void Jet::Accelerate(const INT& accelerate)
{
	if (accelerate > 0)
		m_objectiveSpeed += 0.04;
	else if (accelerate < 0)
		m_objectiveSpeed -= 0.02;

	m_objectiveSpeed -= (m_objectiveSpeed - 0.5f) * 0.1f;
	m_speed += (m_objectiveSpeed - m_speed) * 0.1f;

	D3DXMATRIX rotation;
	D3DXVECTOR3 direction;
	D3DXMatrixRotationQuaternion(&rotation, &(m_rollRotation * m_yawRotation));
	D3DXVec3TransformCoord(&direction, &D3DXVECTOR3(0, 0, 1), &rotation);
	m_position += direction * m_speed;
}
