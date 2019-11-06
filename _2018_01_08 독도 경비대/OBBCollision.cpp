#include "stdafx.h"
#include "OBBCollision.h"

struct TEMP_V
{
	D3DXVECTOR3 p;
	D3DCOLOR c;
	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
};



OBBCollision::OBBCollision()
	: m_centerPosition(0, 0, 0)
	, m_size(0, 0, 0)
	, m_rotation(0, 0, 0, 1)
{
}
OBBCollision::~OBBCollision()
{
}



void OBBCollision::Update(const D3DXVECTOR3 * centerPosition, const D3DXVECTOR3 * size, const D3DXQUATERNION * rotation)
{
	if (centerPosition)	m_centerPosition = *centerPosition;
	if (size)			m_size = *size;
	if (rotation)		m_rotation = *rotation;
}

void OBBCollision::Render(LPDIRECT3DDEVICE9 device, const D3DXCOLOR& color)
{
	TEMP_V v[8];
	v[0].p = D3DXVECTOR3(-1, 1, 1);
	v[1].p = D3DXVECTOR3(1, 1, 1);
	v[2].p = D3DXVECTOR3(-1, 1, -1);
	v[3].p = D3DXVECTOR3(1, 1, -1);
	v[0 + 4].p = D3DXVECTOR3(-1, -1, 1);
	v[1 + 4].p = D3DXVECTOR3(1, -1, 1);
	v[2 + 4].p = D3DXVECTOR3(-1, -1, -1);
	v[3 + 4].p = D3DXVECTOR3(1, -1, -1);
	v[0].c = v[1].c = v[2].c = v[3].c = v[0 + 4].c = v[1 + 4].c = v[2 + 4].c = v[3 + 4].c = color;
	TEMP_V plane[36];
	plane[0] = v[0];
	plane[1] = v[1];
	plane[2] = v[3];
	plane[3] = v[0];
	plane[4] = v[3];
	plane[5] = v[2];
	plane[0 + 6] = v[5 - 1];
	plane[1 + 6] = v[6 - 1];
	plane[2 + 6] = v[2 - 1];
	plane[3 + 6] = v[5 - 1];
	plane[4 + 6] = v[2 - 1];
	plane[5 + 6] = v[1 - 1];
	plane[0 + 12] = v[6 - 1];
	plane[1 + 12] = v[8 - 1];
	plane[2 + 12] = v[4 - 1];
	plane[3 + 12] = v[6 - 1];
	plane[4 + 12] = v[4 - 1];
	plane[5 + 12] = v[2 - 1];
	plane[0 + 18] = v[8 - 1];
	plane[1 + 18] = v[7 - 1];
	plane[2 + 18] = v[3 - 1];
	plane[3 + 18] = v[8 - 1];
	plane[4 + 18] = v[3 - 1];
	plane[5 + 18] = v[4 - 1];
	plane[0 + 24] = v[7 - 1];
	plane[1 + 24] = v[5 - 1];
	plane[2 + 24] = v[1 - 1];
	plane[3 + 24] = v[7 - 1];
	plane[4 + 24] = v[1 - 1];
	plane[5 + 24] = v[3 - 1];
	plane[0 + 30] = v[5 - 1];
	plane[1 + 30] = v[8 - 1];
	plane[2 + 30] = v[6 - 1];
	plane[3 + 30] = v[5 - 1];
	plane[4 + 30] = v[7 - 1];
	plane[5 + 30] = v[8 - 1];

	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Ambient = mtrl.Diffuse = mtrl.Specular = D3DXCOLOR(1, 1, 1, 1);

	D3DXMATRIX s, r, t;
	D3DXMatrixScaling(&s, m_size.x, m_size.y, m_size.z);
	D3DXMatrixRotationQuaternion(&r, &m_rotation);
	D3DXMatrixTranslation(&t, m_centerPosition.x, m_centerPosition.y, m_centerPosition.z);

	device->SetTransform(D3DTS_WORLD, &(s * r * t));
	device->SetRenderState(D3DRS_LIGHTING, false);
	device->SetFVF(TEMP_V::FVF);
	device->SetMaterial(&mtrl);
	device->SetTexture(0, nullptr);
	HRESULT hr = device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 12, &plane[0], sizeof(TEMP_V));
	device->SetRenderState(D3DRS_LIGHTING, true);
}
bool OBBCollision::CalculateCollision(const OBBCollision & anotherOBB)
{
	//Default
	auto Normalize = [](const D3DXVECTOR3& vector3)->D3DXVECTOR3 { D3DXVECTOR3 normalize; D3DXVec3Normalize(&normalize, &vector3); return normalize; };
	auto Length = [](const D3DXVECTOR3& vector)->float { return D3DXVec3Length(&vector); };
	auto Dot = [](const D3DXVECTOR3& vector1, const D3DXVECTOR3& vector2)->float { return D3DXVec3Dot(&vector1, &vector2); };
	auto Cross = [](const D3DXVECTOR3& vector1, const D3DXVECTOR3& vector2)->D3DXVECTOR3 { D3DXVECTOR3 cross; D3DXVec3Cross(&cross, &vector1, &vector2); return cross; };
	auto Rotation = [](const D3DXVECTOR3& vector3, const D3DXQUATERNION& rotation)->D3DXVECTOR3
	{
		D3DXVECTOR3 rotationVector;
		D3DXMATRIX rotationMatrix;
		D3DXMatrixRotationQuaternion(&rotationMatrix, &rotation);
		D3DXVec3TransformCoord(&rotationVector, &vector3, &rotationMatrix);
		return rotationVector;
	};
	
	auto Rotation_Normailze = [&Normalize, &Rotation](const D3DXVECTOR3& vector, const D3DXQUATERNION& rotation) { return Normalize(Rotation(vector, rotation)); };

	//Professional
	auto ProjectedOBBIntervalLength = [&Dot, &Normalize, &Length](const OBBCollision& thisOBB, const OBBCollision& anotherOBB, const D3DXVECTOR3& axis)->float
	{
		D3DXVECTOR3 intervalVector = anotherOBB.GetCenterPosition() - thisOBB.GetCenterPosition();
		return Length(intervalVector) * fabs(Dot(Normalize(intervalVector), axis));
	};
	auto ProjectedOBBSizeLength = [&Dot, &Rotation, &Normalize, &Length](const OBBCollision& obb, const D3DXVECTOR3& axis)->float
	{
		float _1, _2, _3, _4;
		_1 = fabs(Dot(Rotation(Normalize(D3DXVECTOR3(+obb.GetSize().x, +obb.GetSize().y, +obb.GetSize().z)), obb.GetRotation()), axis));
		_2 = fabs(Dot(Rotation(Normalize(D3DXVECTOR3(+obb.GetSize().x, +obb.GetSize().y, -obb.GetSize().z)), obb.GetRotation()), axis));
		_3 = fabs(Dot(Rotation(Normalize(D3DXVECTOR3(+obb.GetSize().x, -obb.GetSize().y, +obb.GetSize().z)), obb.GetRotation()), axis));
		_4 = fabs(Dot(Rotation(Normalize(D3DXVECTOR3(+obb.GetSize().x, -obb.GetSize().y, -obb.GetSize().z)), obb.GetRotation()), axis));
		float _result1 = (_1 > _2 ? _1 : _2);
		float _result2 = (_3 > _4 ? _3 : _4);
		return Length(obb.GetSize()) * (_result1 > _result2 ? _result1 : _result2);
	};

	auto CommonAxis = [&Rotation_Normailze](D3DXVECTOR3* axis, const D3DXVECTOR3& basicAxis, const D3DXQUATERNION& axisRotation)->bool
	{
		*axis = Rotation_Normailze(basicAxis, axisRotation);
		return true;
	};
	auto CrossAxis = [&Normalize, &Rotation, &Cross](D3DXVECTOR3* axis, const D3DXVECTOR3& axis1, const D3DXVECTOR3& axis2)->bool
	{
		*axis = Cross(axis1, axis2);
		return true;
	};

	auto OBBCollisionCalculate = [&ProjectedOBBIntervalLength, &ProjectedOBBSizeLength](const OBBCollision& thisOBB, const OBBCollision& anotherOBB, const D3DXVECTOR3& axis)->bool
	{
		return ProjectedOBBIntervalLength(thisOBB, anotherOBB, axis) <= ProjectedOBBSizeLength(thisOBB, axis) + ProjectedOBBSizeLength(anotherOBB, axis);
	};

	D3DXVECTOR3 axis;
	return
		(CommonAxis(&axis, D3DXVECTOR3(m_size.x, 0, 0), m_rotation)																												&& OBBCollisionCalculate(*this, anotherOBB, axis)) && 
		(CommonAxis(&axis, D3DXVECTOR3(0, m_size.y, 0), m_rotation)																												&& OBBCollisionCalculate(*this, anotherOBB, axis)) && 
		(CommonAxis(&axis, D3DXVECTOR3(0, 0, m_size.z), m_rotation)																												&& OBBCollisionCalculate(*this, anotherOBB, axis)) &&
		(CommonAxis(&axis, D3DXVECTOR3(anotherOBB.GetSize().x, 0, 0), anotherOBB.GetRotation())																					&& OBBCollisionCalculate(*this, anotherOBB, axis)) &&
		(CommonAxis(&axis, D3DXVECTOR3(0, anotherOBB.GetSize().y, 0), anotherOBB.GetRotation())																					&& OBBCollisionCalculate(*this, anotherOBB, axis)) &&
		(CommonAxis(&axis, D3DXVECTOR3(0, 0, anotherOBB.GetSize().z), anotherOBB.GetRotation())																					&& OBBCollisionCalculate(*this, anotherOBB, axis)) &&
		(CrossAxis(&axis, Rotation_Normailze(D3DXVECTOR3(m_size.x, 0, 0), m_rotation), Rotation_Normailze(D3DXVECTOR3(anotherOBB.GetSize().x, 0, 0), anotherOBB.GetRotation()))	&& OBBCollisionCalculate(*this, anotherOBB, axis)) &&
		(CrossAxis(&axis, Rotation_Normailze(D3DXVECTOR3(m_size.x, 0, 0), m_rotation), Rotation_Normailze(D3DXVECTOR3(0, anotherOBB.GetSize().y, 0), anotherOBB.GetRotation()))	&& OBBCollisionCalculate(*this, anotherOBB, axis)) &&
		(CrossAxis(&axis, Rotation_Normailze(D3DXVECTOR3(m_size.x, 0, 0), m_rotation), Rotation_Normailze(D3DXVECTOR3(0, 0, anotherOBB.GetSize().z), anotherOBB.GetRotation()))	&& OBBCollisionCalculate(*this, anotherOBB, axis)) &&
		(CrossAxis(&axis, Rotation_Normailze(D3DXVECTOR3(0, m_size.x, 0), m_rotation), Rotation_Normailze(D3DXVECTOR3(anotherOBB.GetSize().x, 0, 0), anotherOBB.GetRotation()))	&& OBBCollisionCalculate(*this, anotherOBB, axis)) &&
		(CrossAxis(&axis, Rotation_Normailze(D3DXVECTOR3(0, m_size.x, 0), m_rotation), Rotation_Normailze(D3DXVECTOR3(0, anotherOBB.GetSize().y, 0), anotherOBB.GetRotation()))	&& OBBCollisionCalculate(*this, anotherOBB, axis)) &&
		(CrossAxis(&axis, Rotation_Normailze(D3DXVECTOR3(0, m_size.x, 0), m_rotation), Rotation_Normailze(D3DXVECTOR3(0, 0, anotherOBB.GetSize().z), anotherOBB.GetRotation()))	&& OBBCollisionCalculate(*this, anotherOBB, axis)) &&
		(CrossAxis(&axis, Rotation_Normailze(D3DXVECTOR3(0, 0, m_size.x), m_rotation), Rotation_Normailze(D3DXVECTOR3(anotherOBB.GetSize().x, 0, 0), anotherOBB.GetRotation()))	&& OBBCollisionCalculate(*this, anotherOBB, axis)) &&
		(CrossAxis(&axis, Rotation_Normailze(D3DXVECTOR3(0, 0, m_size.x), m_rotation), Rotation_Normailze(D3DXVECTOR3(0, anotherOBB.GetSize().y, 0), anotherOBB.GetRotation()))	&& OBBCollisionCalculate(*this, anotherOBB, axis)) &&
		(CrossAxis(&axis, Rotation_Normailze(D3DXVECTOR3(0, 0, m_size.x), m_rotation), Rotation_Normailze(D3DXVECTOR3(0, 0, anotherOBB.GetSize().z), anotherOBB.GetRotation()))	&& OBBCollisionCalculate(*this, anotherOBB, axis));
}
