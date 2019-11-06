#include "DXUT.h"
#include "Skybox.h"


Skybox::Skybox()
	//: m_ase(ASELoad(L"./Resource/model", L"Skybox.ase", L"Skybox"))
	: m_ase(ASELoad(L"./Resource/model", L"SkySphere.ase", L"SkySphere"))
{
}
Skybox::~Skybox()
{
}



void Skybox::Render()
{
	D3DXMATRIX viewM;
	DEVICE->GetTransform(D3DTS_VIEW, &viewM);

	D3DXMATRIX sm, tm;
	D3DXMatrixScaling(&sm, 1, 1, 1);
	D3DXMatrixTranslation(&tm, viewM._14, viewM._24, viewM._34);
	DEVICE->SetTransform(D3DTS_WORLD, &(sm * tm));

	m_ase->Render();
}
