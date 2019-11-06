#include "DXUT.h"
#include "Plane.h"



Plane::Plane()
{
	m_plane[0] = PlaneVertex(D3DXVECTOR3(-0.5, +0.5, 0), D3DXVECTOR3(0, 0, -1), D3DXVECTOR2(0, 0));
	m_plane[1] = PlaneVertex(D3DXVECTOR3(+0.5, +0.5, 0), D3DXVECTOR3(0, 0, -1), D3DXVECTOR2(1, 0));
	m_plane[2] = PlaneVertex(D3DXVECTOR3(-0.5, -0.5, 0), D3DXVECTOR3(0, 0, -1), D3DXVECTOR2(0, 1));
	m_plane[3] = PlaneVertex(D3DXVECTOR3(+0.5, -0.5, 0), D3DXVECTOR3(0, 0, -1), D3DXVECTOR2(1, 1));
}
Plane::~Plane()
{
}



void Plane::Render(LPDIRECT3DDEVICE9 device)
{
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &m_plane[0], sizeof(PlaneVertex));
}