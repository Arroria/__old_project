#include "stdafx.h"
#include "Vulkan.h"

#include "OBBCollision.h"


Vulkan::Vulkan()
	: m_model(nullptr)

	, m_position(0, 0, 0)
	, m_direction(0, 0, 1)

	, m_speed(1)
	, m_duraction(180)

	, m_delete(false)
	, m_obb(nullptr)
{
}
Vulkan::~Vulkan()
{
}



void Vulkan::Initialize(ASE * model, const D3DXVECTOR3 & position, const D3DXQUATERNION& rotation, const float & speed)
{
	m_model			= model;
	m_position		= position;
	m_speed			= speed;

	D3DXMatrixRotationQuaternion(&m_rotation, &rotation);
	D3DXVec3TransformCoord(&m_direction, &D3DXVECTOR3(0, 0, 1), &m_rotation);

	m_obb = new OBBCollision();
	m_obb->Update(nullptr, &D3DXVECTOR3(0.05, 0.05, 0.4), &rotation);
}

void Vulkan::Update()
{
	m_duraction--;
	if (m_duraction <= 0)
	{
		m_delete = true;
		return;
	}

	m_position += m_direction * m_speed;


	m_obb->Update(&m_position, nullptr, nullptr);
}

void Vulkan::Render()
{
	D3DXMATRIX translationMatrix;
	D3DXMatrixTranslation(&translationMatrix, m_position.x, m_position.y, m_position.z);

	m_model->Render(m_rotation * translationMatrix);
}

void Vulkan::Release()
{
	SAFE_DELETE(m_obb);
}
