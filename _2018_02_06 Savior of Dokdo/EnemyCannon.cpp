#include "stdafx.h"
#include "EnemyCannon.h"

#include "Frame.h"
#include "Player.h"

EnemyCannon::EnemyCannon(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir)
	: m_pos(pos)
	, m_dir(dir)
	, m_rotQ(0, 0, 0, 1)

	, m_duration(300)
{
	D3DXMATRIX rotM;
	D3DXMatrixLookAtLH(&rotM, &D3DXVECTOR3(0, 0, 0), &m_dir, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixTranspose(&rotM, &rotM);
	D3DXQuaternionRotationMatrix(&m_rotQ, &rotM);
}
EnemyCannon::~EnemyCannon()
{
}



void EnemyCannon::Update(Player* player)
{
	if (!m_duration)
	{
		Expire();
		return;
	}
	m_duration--;
	m_pos += m_dir * 3;


	constexpr FLOAT DISTANCE = 1;
	constexpr FLOAT DISTANCE_SQ = DISTANCE * DISTANCE;
	if (D3DXVec3LengthSq(&(player->GetFrame()->GetPosition() - m_pos)) < DISTANCE_SQ)
	{
		player->Hit();
		Expire();
		return;
	}
}

void EnemyCannon::Render()
{
	D3DXMATRIX planeRot;
	D3DXMatrixRotationX(&planeRot, D3DXToRadian(90));

	D3DXMATRIX rm, tm;
	D3DXMatrixRotationQuaternion(&rm, &m_rotQ);
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);

	D3DXMATRIX rms;
	{
		D3DXVECTOR3 camDir(m_pos - GetSingleton(CameraManager)->GetData().position);
		D3DXVec3Normalize(&camDir, &camDir);

		D3DXMATRIX temp;
		D3DXMatrixLookAtLH(&temp, &D3DXVECTOR3(0, 0, 0), &m_dir, &D3DXVECTOR3(0, 1, 0));

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


	constexpr FLOAT SCALE = 0.04;

	Texture* tex = GetSingleton(TextureLoader)->LoadTexture(L"./Resource/attack.png", L"Attack");
	D3DXMATRIX sm;
	D3DXMatrixScaling(&sm, tex->imageInfo.Width * SCALE, tex->imageInfo.Height * SCALE, 0);

	DEVICE->SetTransform(D3DTS_WORLD, &(sm * planeRot * rms * rm * tm));
	DEVICE->SetTexture(0, tex->texture);
	DEVICE->SetFVF(GetSingleton(Plane)->GetFVF());
	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	GetSingleton(Plane)->Render(DEVICE);
	DEVICE->SetRenderState(D3DRS_LIGHTING, true);
}