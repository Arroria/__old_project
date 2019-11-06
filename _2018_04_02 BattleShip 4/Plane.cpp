#include "DXUT.h"
#include "Plane.h"


Plane::Plane()
{
	m_vertex[0] = PlaneVertex(D3DXVECTOR3(-0.5f, +0.5f, 0), D3DXVECTOR3(0, 0, -1), D3DXVECTOR2(0, 0));
	m_vertex[1] = PlaneVertex(D3DXVECTOR3(+0.5f, +0.5f, 0), D3DXVECTOR3(0, 0, -1), D3DXVECTOR2(1, 0));
	m_vertex[2] = PlaneVertex(D3DXVECTOR3(-0.5f, -0.5f, 0), D3DXVECTOR3(0, 0, -1), D3DXVECTOR2(0, 1));
	m_vertex[3] = PlaneVertex(D3DXVECTOR3(+0.5f, -0.5f, 0), D3DXVECTOR3(0, 0, -1), D3DXVECTOR2(1, 1));
}
Plane::~Plane()
{
}



HRESULT Plane::Render()
{
	DEVICE->SetFVF(PlaneVertex::FVF);
	return DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &m_vertex[0], sizeof(PlaneVertex));
}
