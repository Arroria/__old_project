#include "stdafx.h"
#include "DestroyedAirPlane.h"


DestroyedAirPlane::DestroyedAirPlane(ASE* model, const D3DXVECTOR3 position)
	: m_model(model)

	, m_position(position)
	, m_rotation(0, 0, 0, 1)
	, m_rotationDelta(0, 0, 0, 1)
	, m_dir(0, 0, 0)
	, m_movePower((float)(rand() % 100) / 50)

	, m_gravity(0.1f)
{
	m_dir = D3DXVECTOR3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50);
	D3DXVec3Normalize(&m_dir, &m_dir);
	D3DXQuaternionRotationAxis(&m_rotationDelta, &m_dir, D3DXToRadian( rand() % 360 ));
}
DestroyedAirPlane::~DestroyedAirPlane()
{
}



void DestroyedAirPlane::Update()
{
	m_rotation *= m_rotationDelta;
	m_gravity *= 1.02f;
	
	m_position += m_dir * m_movePower * 0.3f;
	m_position.y -= m_gravity;
	
	if (m_position.y < -10)
		Expire();
}

void DestroyedAirPlane::Render()
{
	D3DXMATRIX rm, tm;
	D3DXMatrixRotationQuaternion(&rm, &m_rotation);
	D3DXMatrixTranslation(&tm, m_position.x, m_position.y, m_position.z);

	m_model->SetFVF(DEVICE);
	UINT attributeCount = m_model->GetAttributeCount();
	for (UINT attribute = 0; attribute < attributeCount; attribute++)
	{
		GeomObj* geomObj = m_model->GetGeomObj(attribute);
		GetSingleton(TempShaderHelper)->SetMaterial("g_material", geomObj->mtrlTex->first);
		GetSingleton(TempShaderHelper)->SetTexture("g_texture", geomObj->mtrlTex->second);
		DEVICE->SetTransform(D3DTS_WORLD, &(geomObj->localMatrixSR * rm * geomObj->localMatrixT * tm));
	
		UINT passCount = GetSingleton(TempShaderHelper)->Begin(DEVICE);
		for (UINT pass = 0; pass < passCount; pass++)
		{
			GetSingleton(TempShaderHelper)->BeginDraw(pass);
			m_model->Render(DEVICE, attribute);
			GetSingleton(TempShaderHelper)->EndDraw();
		}
		GetSingleton(TempShaderHelper)->End();
	}
}
