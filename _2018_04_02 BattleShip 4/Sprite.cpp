#include "DXUT.h"
#include "Sprite.h"


Sprite::Sprite()
{
	D3DXCreateSprite(DEVICE, &m_sprite);
}
Sprite::~Sprite()
{
	SAFE_RELEASE(m_sprite);
}
