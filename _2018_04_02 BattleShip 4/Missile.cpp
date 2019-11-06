#include "DXUT.h"
#include "Missile.h"


Missile::Missile(const POINT& lockonPos, const bool& byPlayer)
	: m_pos(0, 0, 0)
	, m_dir(0, 0, 1)
	, m_terminal(0, 0, 0)
	, m_isDiving(false)

	, m_lockonPos(lockonPos)
	, m_byPlayer(byPlayer)
{
}
Missile::~Missile()
{
}



void Missile::Update()
{
	constexpr FLOAT SPEED = 0.6;
	constexpr FLOAT ROTATE_SPEED = 0.1;

	D3DXMATRIX rm1, rm2;
	D3DXQUATERNION rq1, rq2;
	D3DXMatrixLookAtLH(&rm1, &D3DXVECTOR3(0, 0, 0), &m_dir, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixLookAtLH(&rm2, &D3DXVECTOR3(0, 0, 0), &(m_terminal - m_pos), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixTranspose(&rm1, &rm1);
	D3DXMatrixTranspose(&rm2, &rm2);
	D3DXQuaternionRotationMatrix(&rq1, &rm1);
	D3DXQuaternionRotationMatrix(&rq2, &rm2);

	D3DXQUATERNION rq;
	D3DXMATRIX rm;
	D3DXQuaternionSlerp(&rq, &rq1, &rq2, ROTATE_SPEED);
	D3DXMatrixRotationQuaternion(&rm, &rq);
	D3DXVec3TransformNormal(&m_dir, &D3DXVECTOR3(0, 0, 1), &rm);

	m_pos += m_dir * SPEED;
}

void Missile::Render()
{
	constexpr FLOAT SCALE = 1;

	D3DXMATRIX sm;
	D3DXMatrixScaling(&sm, SCALE, SCALE, SCALE);

	D3DXMATRIX rm;
	D3DXMatrixLookAtLH(&rm, &D3DXVECTOR3(0, 0, 0), &m_dir, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixTranspose(&rm, &rm);

	D3DXMATRIX tm;
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);

	DEVICE->SetTransform(D3DTS_WORLD, &(sm * rm * tm));
	ASEGet(L"Missile")->Render();
}



void Missile::SetState(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & dir, const D3DXVECTOR3 & terminal, const bool & isDiving)
{
	m_pos		= pos;
	m_dir		= dir;
	m_terminal	= terminal;
	m_isDiving	= isDiving;
}
