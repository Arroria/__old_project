#pragma once
#include "Singleton.h"
class WhiteMtrl :
	public Singleton<WhiteMtrl>
{
private:
	D3DMATERIAL9 m_mtrl;

public:
	D3DMATERIAL9 GetMtrl();

public:
	WhiteMtrl();
	~WhiteMtrl();
};

#define WHITEMTRL (SingletonInstance(WhiteMtrl)->GetMtrl())
