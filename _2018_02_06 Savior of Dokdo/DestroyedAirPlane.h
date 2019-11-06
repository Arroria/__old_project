#pragma once
#include "Effect3D.h"
class DestroyedAirPlane :
	public Effect3D
{
private:
	ASE* m_model;

	D3DXVECTOR3 m_position;
	D3DXQUATERNION m_rotation;
	D3DXQUATERNION m_rotationDelta;

	D3DXVECTOR3 m_dir;
	float m_movePower;
	float m_gravity;

public:
	void Update() override;
	void Render() override;

public:
	DestroyedAirPlane(ASE* model, const D3DXVECTOR3 position);
	~DestroyedAirPlane();
};

