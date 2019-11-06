#include "stdafx.h"
#include "Bullet.h"

#include "Enemy.h"
#include "Frame.h"

Bullet::Bullet(const D3DXVECTOR3& position, Enemy* target)
	: m_position(position)
	, m_rotation(0, 0, 0, 1)

	, m_targetPosition(0, 0, 0)
	, m_direction(0, 0, 0)

	, m_target(target)

	, m_isExpired(false)
{
	m_target->AddReferencer(&m_target);

	m_targetPosition = m_target->GetFrame()->GetPosition();
	D3DXVec3Normalize(&m_direction, &(m_targetPosition - m_position));
	D3DXMATRIX temp;
	D3DXMatrixLookAtLH(&temp, &D3DXVECTOR3(0, 0, 0), &m_direction, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixTranspose(&temp, &temp);
	D3DXQuaternionRotationMatrix(&m_rotation, &temp);
}
Bullet::~Bullet()
{
}



void Bullet::Initialize()
{
}

void Bullet::Update()
{
	constexpr FLOAT SPEED = 5;

	if (m_target)
	{
		float speed = m_target->GetFrame()->GetSpeed();
		m_targetPosition = m_target->GetFrame()->GetPosition();
		D3DXVec3Normalize(&m_direction, &(m_targetPosition - m_position));
	
		D3DXMATRIX temp;
		D3DXMatrixLookAtLH(&temp, &D3DXVECTOR3(0, 0, 0), &m_direction, &D3DXVECTOR3(0, 1, 0));
		D3DXMatrixTranspose(&temp, &temp);
		D3DXQuaternionRotationMatrix(&m_rotation, &temp);
	}
	
	m_position += m_direction * SPEED;
	
	if (D3DXVec3Length(&(m_targetPosition - m_position)) <= SPEED * 0.5f)
	{
		if (m_target)
			m_target->Hit();
		m_isExpired = true;
	}
}

void Bullet::Render(ASE* model)
{
	D3DXMATRIX planeRot;
	D3DXMatrixRotationX(&planeRot, D3DXToRadian(90));

	D3DXMATRIX rm, tm;
	D3DXMatrixRotationQuaternion(&rm, &m_rotation);
	D3DXMatrixTranslation(&tm, m_position.x, m_position.y, m_position.z);

	D3DXMATRIX rms;
	{
		D3DXVECTOR3 camDir(m_position - GetSingleton(CameraManager)->GetData().position);
		D3DXVec3Normalize(&camDir, &camDir);

		D3DXMATRIX temp;
		D3DXMatrixLookAtLH(&temp, &D3DXVECTOR3(0, 0, 0), &m_direction, &D3DXVECTOR3(0, 1, 0));

		D3DXVECTOR3 coordCameraDir;
		D3DXVec3TransformNormal(&coordCameraDir, &-camDir, &temp);
		D3DXVec3Normalize(&coordCameraDir, &D3DXVECTOR3(coordCameraDir.x, coordCameraDir.y, 0));

		float zRotRadian = acos(D3DXVec3Dot(&D3DXVECTOR3(0, 1, 0), &coordCameraDir));
		D3DXVECTOR3 crossResult;
		D3DXVec3Cross(&crossResult, &D3DXVECTOR3(0, 1, 0), &coordCameraDir);
		if (crossResult.z < 0)
			zRotRadian = D3DXToRadian(360) - zRotRadian;

		D3DXMatrixRotationZ(&rms, zRotRadian);
	}



	Texture* tex = GetSingleton(TextureLoader)->LoadTexture(L"./Resource/attack.png", L"Attack");
	D3DXMATRIX sm;
	D3DXMatrixScaling(&sm, tex->imageInfo.Width * 0.1, tex->imageInfo.Height * 0.1, 0);
	
	DEVICE->SetTransform(D3DTS_WORLD, &(sm * planeRot * rms * rm * tm));
	DEVICE->SetTexture(0, tex->texture);
	DEVICE->SetFVF(GetSingleton(Plane)->GetFVF());
	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	GetSingleton(Plane)->Render(DEVICE);
	DEVICE->SetRenderState(D3DRS_LIGHTING, true);
}
