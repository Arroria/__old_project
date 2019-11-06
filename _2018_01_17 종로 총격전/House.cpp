#include "stdafx.h"
#include "House.h"


House::House()
	: m_position(0, 0)
	, m_tex(nullptr)
{
}
House::~House()
{
}



void House::Initialize(const D3DXVECTOR2& position, const UINT& type)
{
	m_tex = GetSingleton(TextureLoader)->GetTexture(std::wstring(L"House") + std::to_wstring(type));
	m_position = position;

	D3DXQUATERNION _2, _3;
	D3DXQuaternionRotationAxis(&_2, &D3DXVECTOR3(0, 0, 1), -D3DXToRadian(20));
	D3DXQuaternionRotationAxis(&_3, &D3DXVECTOR3(0, 0, 1), +D3DXToRadian(20));

	m_collisionList.Clear();
	m_collisionList.InsertVertex(D3DXVECTOR2(m_position.x - (298 * 0.5f),		m_position.y + (273 * 0.5f) - 76),	D3DXVECTOR2(m_position.x - (298 * 0.5f) + 45,	m_position.y + (273 * 0.5f)));
	m_collisionList.InsertVertex(D3DXVECTOR2(m_position.x - (298 * 0.5f) + 45,	m_position.y + (273 * 0.5f)),		D3DXVECTOR2(m_position.x + (298 * 0.5f) - 45,	m_position.y + (273 * 0.5f)));
	m_collisionList.InsertVertex(D3DXVECTOR2(m_position.x + (298 * 0.5f) - 45,	m_position.y + (273 * 0.5f)),		D3DXVECTOR2(m_position.x + (298 * 0.5f),		m_position.y + (273 * 0.5f) - 76));

	switch (type)
	{
	case 0:
		m_collisionList.InsertVertex(D3DXVECTOR2(m_position.x - (298 * 0.5f) + 4, m_position.y + (273 * 0.5f) - 173),	D3DXVECTOR2(m_position.x - (298 * 0.5f) + 14, m_position.y + (273 * 0.5f) - 164));
		m_collisionList.InsertVertex(D3DXVECTOR2(m_position.x - (298 * 0.5f) + 14, m_position.y + (273 * 0.5f) - 164),	D3DXVECTOR2(m_position.x + (298 * 0.5f) - 14, m_position.y + (273 * 0.5f) - 164));
		m_collisionList.InsertVertex(D3DXVECTOR2(m_position.x + (298 * 0.5f) - 14, m_position.y + (273 * 0.5f) - 164),	D3DXVECTOR2(m_position.x + (298 * 0.5f) - 4, m_position.y + (273 * 0.5f) - 173));
		break;
		
	case 1:
		m_collisionList.InsertVertex(D3DXVECTOR2(m_position.x - (298 * 0.5f) + 11, m_position.y + (273 * 0.5f) - 175), D3DXVECTOR2(m_position.x - (298 * 0.5f) + 54, m_position.y + (273 * 0.5f) - 123));
		m_collisionList.InsertVertex(D3DXVECTOR2(m_position.x - (298 * 0.5f) + 54, m_position.y + (273 * 0.5f) - 123), D3DXVECTOR2(m_position.x - (298 * 0.5f) + 98, m_position.y + (273 * 0.5f) - 175));
		m_collisionList.InsertVertex(D3DXVECTOR2(m_position.x + (298 * 0.5f) - 54, m_position.y + (273 * 0.5f) - 123), D3DXVECTOR2(m_position.x + (298 * 0.5f) - 11, m_position.y + (273 * 0.5f) - 175));
		m_collisionList.InsertVertex(D3DXVECTOR2(m_position.x + (298 * 0.5f) - 98, m_position.y + (273 * 0.5f) - 175), D3DXVECTOR2(m_position.x + (298 * 0.5f) - 54, m_position.y + (273 * 0.5f) - 123));
		break;
	case 2:
		m_collisionList.InsertVertex(D3DXVECTOR2(m_position.x - (298 * 0.5f),		m_position.y + (273 * 0.5f) - 114), D3DXVECTOR2(m_position.x - (298 * 0.5f) + 18,	m_position.y + (273 * 0.5f) - 118));
		m_collisionList.InsertVertex(D3DXVECTOR2(m_position.x - (298 * 0.5f) + 18,	m_position.y + (273 * 0.5f) - 118), D3DXVECTOR2(m_position.x + (298 * 0.5f) - 18,	m_position.y + (273 * 0.5f) - 118));
		m_collisionList.InsertVertex(D3DXVECTOR2(m_position.x + (298 * 0.5f) - 18,	m_position.y + (273 * 0.5f) - 118), D3DXVECTOR2(m_position.x + (298 * 0.5f),		m_position.y + (273 * 0.5f) - 114));
		break;
	} 
}

void House::Update()
{
}

void House::Render()
{
	GetSingleton(PlaneRenderer)->Render(DEVICE, m_tex, D3DXVECTOR3(m_position.x, m_position.y, 10));
}

void House::Release()
{
}
