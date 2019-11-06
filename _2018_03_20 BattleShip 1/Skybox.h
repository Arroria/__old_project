#pragma once
#include "Singleton.h"
class Skybox :
	public Singleton<Skybox>
{
private:
	EasyASE* m_ase;

public:
	void Render();

public:
	Skybox();
	~Skybox();
};

