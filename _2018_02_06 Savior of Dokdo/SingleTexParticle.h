#pragma once
#include "Effect3D.h"
class SingleTexParticle :
	public Effect3D
{
protected:
	D3DXVECTOR3 m_pos;
	FLOAT m_rotDegree;
	D3DXVECTOR3 m_scale;

	Texture* m_texture;
	D3DMATERIAL9 m_mtrl;

public:
	virtual void Update() PURE;
	virtual void Render() override;

public:
	SingleTexParticle();
	~SingleTexParticle();
};

