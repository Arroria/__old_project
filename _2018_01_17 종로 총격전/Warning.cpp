#include "stdafx.h"
#include "Warning.h"


Warning::Warning()
	: m_time(0)
{
}
Warning::~Warning()
{
}



void Warning::Initialize()
{
	GetSingleton(TextureLoader)->RegisterTexture(DEVICE, L"Warning", L"./Resource/ui/warning.png");
}

void Warning::Update()
{
	m_time++;
}

void Warning::Render(const D3DXVECTOR3& cameraPosition)
{
	int a = 0;
	if (m_time < 60)
		a = 450 - m_time * 5;
	else if (m_time > 240)
		a = 450 - (300 - m_time) * 5;
	else
		a = 150;

	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"Warning"), cameraPosition + D3DXVECTOR3((int)m_time - 1537.5,		a, 0),	false, D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(0.5, 0.5));
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"Warning"), cameraPosition + D3DXVECTOR3((int)m_time - 1025,		a, 0),	false, D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(0.5, 0.5));
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"Warning"), cameraPosition + D3DXVECTOR3((int)m_time - 512.5,		a, 0),	false, D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(0.5, 0.5));
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"Warning"), cameraPosition + D3DXVECTOR3((int)m_time,				a, 0),	false, D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(0.5, 0.5));
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"Warning"), cameraPosition + D3DXVECTOR3((int)m_time + 512.5,		a, 0),	false, D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(0.5, 0.5));
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"Warning"), cameraPosition + D3DXVECTOR3(-(int)m_time - 512.5,		-a, 0),	false, D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(0.5, 0.5));
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"Warning"), cameraPosition + D3DXVECTOR3(-(int)m_time,				-a, 0),	false, D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(0.5, 0.5));
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"Warning"), cameraPosition + D3DXVECTOR3(-(int)m_time + 512.5,		-a, 0),	false, D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(0.5, 0.5));
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"Warning"), cameraPosition + D3DXVECTOR3(-(int)m_time + 1025,		-a, 0),	false, D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(0.5, 0.5));
	GetSingleton(PlaneRenderer)->Render(DEVICE, GetSingleton(TextureLoader)->GetTexture(L"Warning"), cameraPosition + D3DXVECTOR3(-(int)m_time + 1537.5,	-a, 0),	false, D3DXQUATERNION(0, 0, 0, 1), D3DXVECTOR2(0.5, 0.5));
}

void Warning::Release()
{
}
