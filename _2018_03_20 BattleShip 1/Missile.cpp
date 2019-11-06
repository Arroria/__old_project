#include "DXUT.h"
#include "Missile.h"

#include "MissileTail.h"


Missile::Missile(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & dir, const D3DXVECTOR3& objectivePos, const D3DXVECTOR3& terminal, const bool& byPlayer, const bool& isHit)
	: m_isExpired(false)

	, m_pos(pos)
	, m_dir(dir)
	, m_objectivePos(objectivePos)
	, m_terminal(terminal)
	, m_byPlayer(byPlayer)
	, m_diving(false)
	, m_hitEffect(isHit)
	
	, m_speed(rand() % 100 / 100. * 0.2f + 0.9)
{
}
Missile::~Missile()
{
}



void Missile::Update(std::list<Effect*>& effectList)
{
	constexpr FLOAT MOVEMENT_SPEED = 0.5f;
	constexpr FLOAT ROTATION_SPEED = 0.1;

	D3DXQUATERNION dirQ; {
		D3DXQUATERNION v1Q, v2Q;
		D3DXMATRIX v1RM, v2RM; {
			D3DXVECTOR3 objDir;
			D3DXVec3Normalize(&objDir, &(m_objectivePos - m_pos));
			D3DXMatrixLookAtLH(&v1RM, &D3DXVECTOR3(0, 0, 0), &m_dir, &D3DXVECTOR3(0, 1, 0));
			D3DXMatrixLookAtLH(&v2RM, &D3DXVECTOR3(0, 0, 0), &objDir, &D3DXVECTOR3(0, 1, 0));
			D3DXMatrixTranspose(&v1RM, &v1RM);
			D3DXMatrixTranspose(&v2RM, &v2RM);
		}
		D3DXQuaternionRotationMatrix(&v1Q, &v1RM);
		D3DXQuaternionRotationMatrix(&v2Q, &v2RM);
		D3DXQuaternionSlerp(&dirQ, &v1Q, &v2Q, ROTATION_SPEED);
	}
	D3DXMATRIX dirRM;
	D3DXMatrixRotationQuaternion(&dirRM, &dirQ);
	D3DXVec3TransformNormal(&m_dir, &D3DXVECTOR3(0, 0, 1), &dirRM);
	D3DXVec3Normalize(&m_dir, &m_dir);

	m_pos += m_dir * MOVEMENT_SPEED * m_speed;

	if (m_byPlayer || m_diving)
		effectList.push_back(new MissileTail(m_pos, m_dir));
}

void Missile::Render()
{
	constexpr FLOAT SCALE = 2.f;

	D3DXMATRIX sm, rm, tm;
	D3DXMatrixScaling(&sm, SCALE, SCALE, SCALE);
	D3DXMatrixLookAtLH(&rm, &D3DXVECTOR3(0, 0, 0), &m_dir, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixTranspose(&rm, &rm);
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);

	DEVICE->SetTransform(D3DTS_WORLD, &(sm * rm * tm));
	ASEGet(L"missile")->Render();
}
