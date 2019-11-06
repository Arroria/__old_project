#include "stdafx.h"
#include "YamatoTurret.h"

#include "EnemyCannon.h"

YamatoTurret::YamatoTurret(const D3DXVECTOR3 & pos, const D3DXQUATERNION& rot)
	: m_attackCooltime(0)
	, m_attackDuration(5)
{
	m_pos = pos;
	m_rot = rot;
}
YamatoTurret::~YamatoTurret()
{
}



void YamatoTurret::Initialize()
{
}

void YamatoTurret::Update(const D3DXVECTOR3& pos, const D3DXQUATERNION& rot, std::vector<EnemyAttack*>* attackContainer, const D3DXVECTOR3 & playerPos)
{
	D3DXVECTOR3 myPos(m_pos + pos);

	D3DXVECTOR3 dir(playerPos - myPos);
	D3DXVec3Normalize(&dir, &dir);

	D3DXMATRIX m;
	D3DXMatrixRotationQuaternion(&m, &(rot * m_rot));
	D3DXVECTOR3 tmp;
	D3DXVec3TransformNormal(&tmp, &D3DXVECTOR3(1, 0, 0), &m);

	if (D3DXVec3Dot(&tmp, &dir) > cos(D3DXToRadian(30)) && m_attackDuration && !m_attackCooltime)
	{
			D3DXVECTOR3 myDir(dir);

			D3DXVECTOR3 attackDir(playerPos - myPos);
			D3DXVec3Normalize(&attackDir, &attackDir);

			if (D3DXVec3Dot(&attackDir, &myDir) > cos(D3DXToRadian(30)))
			{
				D3DXVECTOR3 attackYawDir; {
					D3DXVec3Cross(&attackYawDir, &attackDir, &D3DXVECTOR3(0, 1, 0));
				}
				D3DXMATRIX rot, dir;
				D3DXMatrixRotationAxis(&rot, &attackYawDir, D3DXToRadian(rand() % 100 * 0.05f));
				D3DXMatrixRotationAxis(&dir, &attackDir, D3DXToRadian(rand() % 360));
				D3DXVec3TransformNormal(&attackDir, &attackDir, &rot);
				D3DXVec3TransformNormal(&attackDir, &attackDir, &dir);


				EnemyCannon* eA = new EnemyCannon(myPos, attackDir);
				attackContainer->push_back(eA);

				m_attackDuration--;
				if (!m_attackDuration)
					m_attackCooltime = 60;
				else
					m_attackCooltime = 3;
			}
	}
	if (m_attackCooltime)
		m_attackCooltime--;
	else if (!m_attackDuration)
		m_attackDuration = 5;
}

void YamatoTurret::Render(const D3DXMATRIX & parentLocalM, const D3DXMATRIX& parentSRM, const D3DXMATRIX& parentTM, ASE* model)
{
	D3DXMATRIX parentInvLocalM, rm, tm;
	D3DXMatrixInverse(&parentInvLocalM, 0, &parentLocalM);
	D3DXMatrixRotationQuaternion(&rm, &m_rot);
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);

	model->SetFVF(DEVICE);
	UINT attributeCount = model->GetAttributeCount();
	for (UINT attribute = 0; attribute < attributeCount; attribute++)
	{
		GeomObj* geomObj = model->GetGeomObj(attribute);
		GetSingleton(TempShaderHelper)->SetMaterial("g_material", geomObj->mtrlTex->first);
		GetSingleton(TempShaderHelper)->SetTexture("g_texture", geomObj->mtrlTex->second);
		DEVICE->SetTransform(D3DTS_WORLD, &(geomObj->localMatrixSR * rm * geomObj->localMatrixT * (parentInvLocalM * parentSRM * parentLocalM) * tm * parentTM));

		UINT passCount = GetSingleton(TempShaderHelper)->Begin(DEVICE);
		for (UINT i = 0; i < passCount; i++)
		{
			GetSingleton(TempShaderHelper)->BeginDraw(i);
			model->Render(DEVICE, attribute);
			GetSingleton(TempShaderHelper)->EndDraw();
		}
		GetSingleton(TempShaderHelper)->End();
	}
}

void YamatoTurret::Release()
{
}
