#include "DXUT.h"
#include "WhiteMaterial.h"


WhiteMaterial::WhiteMaterial()
{
	m_mtrl.Ambient =
		m_mtrl.Diffuse =
		m_mtrl.Specular =
		D3DXCOLOR(1, 1, 1, 1);

}
WhiteMaterial::~WhiteMaterial()
{
}
