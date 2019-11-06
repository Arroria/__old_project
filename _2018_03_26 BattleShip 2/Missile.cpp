#include "DXUT.h"
#include "Missile.h"


Missile::Missile(const POINT& lockonPos, const bool& byPlayer)
	: m_isExpired(false)

	, m_pos(0, 0, 0)
	, m_dir(0, 0, 0)
	, m_lockonPos(lockonPos)
	, m_isFalling(false)

	, m_objPos(0, 0, 0)
	, m_byPlayer(byPlayer)
{
}
Missile::~Missile()
{
}



void Missile::Update()
{
	constexpr FLOAT SPEED = 0.5;
	constexpr FLOAT ROT_SPEED = 0.06;

	D3DXVECTOR3 objDir;
	D3DXVec3Normalize(&objDir, &D3DXVECTOR3(m_objPos - m_pos));

	D3DXMATRIX dirRM, objDirRM;
	D3DXMatrixLookAtLH(&dirRM, &D3DXVECTOR3(0, 0, 0), &m_dir, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixLookAtLH(&objDirRM, &D3DXVECTOR3(0, 0, 0), &objDir, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixTranspose(&dirRM, &dirRM);
	D3DXMatrixTranspose(&objDirRM, &objDirRM);
	D3DXQUATERNION dirQ, objDirQ;
	D3DXQuaternionRotationMatrix(&dirQ, &dirRM);
	D3DXQuaternionRotationMatrix(&objDirQ, &objDirRM);

	D3DXQUATERNION nextDirQ;
	D3DXMATRIX nextDirRM;
	D3DXVECTOR3 nextDir;
	D3DXQuaternionSlerp(&nextDirQ, &dirQ, &objDirQ, ROT_SPEED);
	D3DXMatrixRotationQuaternion(&nextDirRM, &nextDirQ);
	D3DXVec3TransformNormal(&nextDir, &D3DXVECTOR3(0, 0, 1), &nextDirRM);

	m_dir = nextDir;
	

	m_pos += m_dir * SPEED;
}

void Missile::Render()
{
	D3DXMATRIX tm;
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);
	DEVICE->SetTransform(D3DTS_WORLD, &(tm));
	//ASEGet(L"Missile")->Render();
	ASEGet(L"T2")->Render();
}



void Missile::SetPath(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & dir, const D3DXVECTOR3 & objPos)
{
	m_pos = pos;
	m_dir = dir;
	m_objPos = objPos;
}
