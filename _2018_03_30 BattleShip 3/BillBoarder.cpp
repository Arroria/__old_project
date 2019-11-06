#include "DXUT.h"
#include "BillBoarder.h"


BillBoarder::BillBoarder()
{
	ZeroMemory(&m_billBoardM, sizeof(D3DXMATRIX));
}
BillBoarder::~BillBoarder()
{
}



void BillBoarder::Update()
{
	DEVICE->GetTransform(D3DTS_VIEW, &m_billBoardM);
	D3DXMatrixInverse(&m_billBoardM, 0, &m_billBoardM);
	m_billBoardM._41 =
		m_billBoardM._42 =
		m_billBoardM._43 =
		0;
}
