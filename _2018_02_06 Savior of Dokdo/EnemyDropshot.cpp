#include "stdafx.h"
#include "EnemyDropshot.h"

#include "Player.h"
#include "Frame.h"

#include "EnemyCannon.h"
constexpr FLOAT SPEED = 5;

EnemyDropshot::EnemyDropshot(const D3DXVECTOR3 & pos, std::vector<EnemyAttack*>* attackContainer)
	: m_pos(pos)
	, m_duration(15)
	, m_attackContainer(attackContainer)
{
}
EnemyDropshot::~EnemyDropshot()
{
}



void EnemyDropshot::Update(Player * player)
{
	if (!m_duration)
	{
		D3DXVECTOR3 attackDir(player->GetFrame()->GetPosition() - m_pos);
		D3DXVec3Normalize(&attackDir, &attackDir);
		D3DXVECTOR3 attackYawDir; {
			D3DXVec3Cross(&attackYawDir, &attackDir, &D3DXVECTOR3(0, 1, 0));
		}
		D3DXMATRIX rot, dir;
		D3DXMatrixRotationAxis(&rot, &attackYawDir, D3DXToRadian(rand() % 100 * 0.05f));
		D3DXMatrixRotationAxis(&dir, &attackDir, D3DXToRadian(rand() % 360));
		D3DXVec3TransformNormal(&attackDir, &attackDir, &rot);
		D3DXVec3TransformNormal(&attackDir, &attackDir, &dir);


		EnemyCannon* eD = new EnemyCannon(m_pos, attackDir);
		m_attackContainer->push_back(eD);

		Expire();
		return;
	}
	m_duration--;

	constexpr FLOAT SPEED = 0.8;
	m_pos.y += SPEED;
}

void EnemyDropshot::Render()
{
	D3DXMATRIX rm, tm;
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);
	DEVICE->GetTransform(D3DTS_VIEW, &rm);
	rm._41 = rm._42 = rm._43 = 0;
	D3DXMatrixTranspose(&rm, &rm);


	constexpr FLOAT SCALE = 0.01;

	Texture* tex = GetSingleton(TextureLoader)->LoadTexture(L"./Resource/temp/Enemy_Bullet.PNG", L"TEMP");
	D3DXMATRIX sm;
	D3DXMatrixScaling(&sm, tex->imageInfo.Width * SCALE, tex->imageInfo.Height * SCALE, 0);

	DEVICE->SetTransform(D3DTS_WORLD, &(sm * rm * tm));
	DEVICE->SetTexture(0, tex->texture);
	DEVICE->SetFVF(GetSingleton(Plane)->GetFVF());
	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	GetSingleton(Plane)->Render(DEVICE);
	DEVICE->SetRenderState(D3DRS_LIGHTING, true);
}
