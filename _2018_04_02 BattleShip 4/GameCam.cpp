#include "DXUT.h"
#include "GameCam.h"


GameCam::GameCam()
	: m_playerControl(false)

	, m_pos(0, -10, -10)
	, m_at(0, 0, 0)
	, m_dist(15)
	, m_rot(15, 0)

	, m_objAt(0, 0, 0)
	, m_objDist(15)
	, m_objRotX(30)

	, m_camShake(0)
	, m_shaking(0, 0, 0)
{
}
GameCam::~GameCam()
{
}



void GameCam::Update()
{
	if (m_playerControl)
	{
		int mouseWheel = SingletonInstance(InputManager)->GetMouseWheel() / 120.f;

		m_objAt = D3DXVECTOR3(0, 0, 0);
		m_objDist -= mouseWheel;
		
		if (SingletonInstance(InputManager)->IsKeyPressed(VK_MBUTTON))
		{
			POINT mouseDelta = SingletonInstance(InputManager)->GetMouseDelta();
			m_rot.x += mouseDelta.y;
			m_rot.y += mouseDelta.x;
		}
	}
	else
	{
		m_rot.x = m_rot.x + (m_objRotX - m_rot.x) * 0.05f;
		m_rot.y += 0.15f;
	}
	D3DXVec3Lerp(&m_at, &m_at, &m_objAt, 0.05f);
	m_dist = m_dist + (m_objDist - m_dist) * 0.05f;

	if (m_rot.x < 15)
		m_rot.x = 15;
	else if (m_rot.x >= 90)
		m_rot.x = 90 - (1.f / 1024);
	if (m_objDist < 1)
		m_objDist = 1;
	else if (m_objDist > 50)
		m_objDist = 50;

	D3DXMATRIX rxm, rym, tm;
	D3DXMatrixRotationX(&rxm, D3DXToRadian(m_rot.x));
	D3DXMatrixRotationY(&rym, D3DXToRadian(m_rot.y));
	D3DXMatrixTranslation(&tm, m_at.x, m_at.y, m_at.z);
	
	D3DXVec3TransformNormal(&m_pos, &D3DXVECTOR3(0, 0, -m_dist), &(rxm * rym));

	if (m_camShake)
	{
		m_shaking; {
			D3DXMATRIX rm;
			D3DXVECTOR3 nor, axis;
			D3DXVec3Normalize(&nor, &(m_at - m_pos));
			D3DXVec3Cross(&axis, &nor, &D3DXVECTOR3(0, m_camShake / 100., 0));
			D3DXMatrixRotationAxis(&rm, &nor, D3DXToRadian(rand() % 3600 / 10.f));
			D3DXVec3TransformNormal(&m_shaking, &axis, &rm);
		}

		m_camShake -= 1;
	}
	else
		m_shaking = D3DXVECTOR3(0, 0, 0);
}

void GameCam::Render()
{
	SingletonInstance(Camera)->SetPos(m_pos + m_at + m_shaking);
	SingletonInstance(Camera)->SetAt(m_at + m_shaking);
}
