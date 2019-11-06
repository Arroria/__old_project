#pragma once
#include "Singleton.h"
class WhiteMaterial :
	public Singleton<WhiteMaterial>
{
private:
	D3DMATERIAL9 m_mtrl;

public:
	D3DMATERIAL9 GetMtrl() { return m_mtrl; }

public:
	WhiteMaterial();
	~WhiteMaterial();
};

#define SetWhiteMaterial() (DEVICE->SetMaterial(&SingletonInstance(WhiteMaterial)->GetMtrl()))
