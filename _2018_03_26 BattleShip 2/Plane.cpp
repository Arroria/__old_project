#include "DXUT.h"
#include "Plane.h"


Plane::Plane()
{
	m_planeData[0] = PlaneVertex(D3DXVECTOR3(-0.5f, +0.5f, 0), D3DXVECTOR3(0, 0, -1), D3DXVECTOR2(0, 0));
	m_planeData[1] = PlaneVertex(D3DXVECTOR3(+0.5f, +0.5f, 0), D3DXVECTOR3(0, 0, -1), D3DXVECTOR2(1, 0));
	m_planeData[2] = PlaneVertex(D3DXVECTOR3(-0.5f, -0.5f, 0), D3DXVECTOR3(0, 0, -1), D3DXVECTOR2(0, 1));
	m_planeData[3] = PlaneVertex(D3DXVECTOR3(+0.5f, -0.5f, 0), D3DXVECTOR3(0, 0, -1), D3DXVECTOR2(1, 1));
}
Plane::~Plane()
{
}



D3DXMATRIX Plane::PivotMatrix(const D3DXVECTOR2 & pivot) { return PivotMatrix(D3DXVECTOR3(pivot.x, pivot.y, 0)); }
D3DXMATRIX Plane::PivotMatrix(const D3DXVECTOR3 & pivot)
{
	D3DXMATRIX pivotM;
	D3DXMatrixTranslation(&pivotM, 0.5f - pivot.x, 0.5f - pivot.y, -pivot.z);
	return pivotM;
}

DWORD Plane::GetFVF() { return PlaneVertex::FVF; }

HRESULT Plane::Render(LPDIRECT3DDEVICE9 device) const
{
	return device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &m_planeData[0], sizeof(PlaneVertex));
}

