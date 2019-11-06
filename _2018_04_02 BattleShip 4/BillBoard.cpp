#include "DXUT.h"
#include "BillBoard.h"


BillBoard::BillBoard()
{
}
BillBoard::~BillBoard()
{
}



D3DXMATRIX BillBoard::Get()
{
	D3DXMATRIX m;
	DEVICE->GetTransform(D3DTS_VIEW, &m);
	D3DXMatrixInverse(&m, 0, &m);
	m._41 =
		m._42 =
		m._43 = 0;
	return m;
}
