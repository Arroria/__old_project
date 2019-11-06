#pragma once
#include "Singleton.h"
class Sprite :
	public Singleton<Sprite>
{
private:
	LPD3DXSPRITE m_sprite;

public:
	LPD3DXSPRITE& GetSprite() { return m_sprite; }

public:
	Sprite();
	~Sprite();
};

#define SPRITE (SingletonInstance(Sprite)->GetSprite())
