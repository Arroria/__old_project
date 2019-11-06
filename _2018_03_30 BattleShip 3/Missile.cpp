#include "DXUT.h"
#include "Missile.h"



Missile::Missile(const POINT & lockonPos, const bool & byPlayer)
	: m_pos(0, 0, 0)
	, m_dir(0, 0, 1)
	, m_terminal(0, 0, 0)

	, m_lockonPos(lockonPos)
	, m_byPlayer(byPlayer)
	, m_isDiving(false)
{
}
Missile::~Missile()
{
}



void Missile::Update()
{
	constexpr FLOAT SLERP_SPEED = 0.1;
	constexpr FLOAT MOVE_SPEED = 1.;

	D3DXMATRIX rm1, rm2;
	D3DXMatrixLookAtLH(&rm1, &D3DXVECTOR3(0, 0, 0), &m_dir, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixLookAtLH(&rm2, &D3DXVECTOR3(0, 0, 0), &[](const D3DXVECTOR3& normal)->D3DXVECTOR3
	{
		D3DXVECTOR3 temp;
		return *D3DXVec3Normalize(&temp, &normal);
	}(m_terminal - m_pos), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixTranspose(&rm1, &rm1);
	D3DXMatrixTranspose(&rm2, &rm2);
	D3DXQUATERNION q1, q2;
	D3DXQuaternionRotationMatrix(&q1, &rm1);
	D3DXQuaternionRotationMatrix(&q2, &rm2);

	D3DXQUATERNION rotQ;
	D3DXQuaternionSlerp(&rotQ, &q1, &q2, SLERP_SPEED);
	D3DXMATRIX rotM;
	D3DXMatrixRotationQuaternion(&rotM, &rotQ);
	D3DXVec3TransformNormal(&m_dir, &D3DXVECTOR3(0, 0, 1), &rotM);

	m_pos += m_dir * MOVE_SPEED;
}

void Missile::Render()
{
	constexpr FLOAT SCALE = 1;

	D3DXMATRIX sm, rm, tm;
	D3DXMatrixScaling(&sm, SCALE, SCALE, SCALE);
	D3DXMatrixLookAtLH(&rm, &D3DXVECTOR3(0, 0, 0), &m_dir, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixTranspose(&rm, &rm);
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);

	DEVICE->SetTransform(D3DTS_WORLD, &(sm * rm * tm));
	ASEGet(L"Missile")->Render();
}



void Missile::Resetting(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & dir, const D3DXVECTOR3& terminal, const bool & isDiving)
{
	m_pos = pos;
	m_dir = dir;
	m_terminal = terminal;
	m_isDiving = isDiving;
}
