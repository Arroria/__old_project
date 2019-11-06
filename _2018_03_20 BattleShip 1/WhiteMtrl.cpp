#include "DXUT.h"
#include "WhiteMtrl.h"


WhiteMtrl::WhiteMtrl()
{
	ZeroMemory(&m_mtrl, sizeof(D3DMATERIAL9));
	m_mtrl.Ambient =
		m_mtrl.Diffuse =
		m_mtrl.Specular =
		D3DXCOLOR(1, 1, 1, 1);
}
WhiteMtrl::~WhiteMtrl()
{
}



D3DMATERIAL9 WhiteMtrl::GetMtrl()
{
	return m_mtrl;
}
