#include "stdafx.h"
#include "OBBCollider.h"


OBBCollider::OBBCollider()
	: m_position(0, 0)
	, m_size(0, 0)
	, m_rotation(0, 0, 0, 1)
{
}
OBBCollider::~OBBCollider()
{
}


void OBBCollider::SetTransform(const D3DXVECTOR2 * position, const D3DXVECTOR2 * size, const D3DXQUATERNION * rotation)
{
	if (position)	m_position = *position;
	if (size)		m_size = *size;
	if (rotation)	m_rotation = *rotation;

}

bool OBBCollider::CalculateCollision(const OBBCollider & anotherOBB)
{
	auto Length		= [](const D3DXVECTOR2& v)->float{ return D3DXVec2Length(&v); };
	auto Dot		= [](const D3DXVECTOR2& v1, const D3DXVECTOR2& v2)->float { return D3DXVec2Dot(&v1, &v2); };
	auto Normalize	= [](const D3DXVECTOR2& v)->D3DXVECTOR2 { D3DXVECTOR2 rv; D3DXVec2Normalize(&rv, &v); return rv; };
	auto Rotation	= [](const D3DXVECTOR2& v, const D3DXQUATERNION& q)->D3DXVECTOR2 { D3DXVECTOR2 rv; D3DXMATRIX r; D3DXMatrixRotationQuaternion(&r, &q); D3DXVec2TransformCoord(&rv, &v, &r); return rv; };

	if (Length(anotherOBB.GetPosition() - m_position) > Length(m_size) + Length(anotherOBB.GetSize()))
		return false;

	auto BiggestDot = [&Dot](const D3DXVECTOR2& axis, const D3DXVECTOR2& transformable)
	{
		float _1, _2;
		_1 = fabs(Dot(axis, D3DXVECTOR2(transformable.x, +transformable.y)));
		_2 = fabs(Dot(axis, D3DXVECTOR2(transformable.x, -transformable.y)));

		return _1 > _2 ? _1 : _2;
	};

	auto ProjectCollisionCalculate = [&Length, &Dot, &Normalize, &Rotation, &BiggestDot](const D3DXVECTOR2& axis, const OBBCollider& thisOBB, const OBBCollider& anotherOBB)
	{
		D3DXVECTOR2 interval = anotherOBB.GetPosition() - thisOBB.GetPosition();

		return fabs(Dot(axis, interval)) * Length(interval) <= BiggestDot(axis, Normalize(Rotation(thisOBB.GetSize(), thisOBB.GetRotation()))) * Length(thisOBB.GetSize()) + BiggestDot(axis, Normalize(Rotation(anotherOBB.GetSize(), anotherOBB.GetRotation()))) * Length(anotherOBB.GetSize());
	};

	return
		ProjectCollisionCalculate(Normalize(Rotation(D3DXVECTOR2(m_size.x, 0), m_rotation)), *this, anotherOBB) &&
		ProjectCollisionCalculate(Normalize(Rotation(D3DXVECTOR2(0, m_size.y), m_rotation)), *this, anotherOBB) &&
		ProjectCollisionCalculate(Normalize(Rotation(D3DXVECTOR2(anotherOBB.GetPosition().x, 0), m_rotation)), *this, anotherOBB) &&
		ProjectCollisionCalculate(Normalize(Rotation(D3DXVECTOR2(0, anotherOBB.GetPosition().y), m_rotation)), *this, anotherOBB);
}

void OBBCollider::RenderBox(LPDIRECT3DDEVICE9 device)
{
	GetSingleton(PlaneRenderer)->Render(device, GetSingleton(TextureLoader)->GetTexture(L"OBB"), D3DXVECTOR3(m_position.x, m_position.y, 0), false, m_rotation, m_size * 0.125f * 2);
}
