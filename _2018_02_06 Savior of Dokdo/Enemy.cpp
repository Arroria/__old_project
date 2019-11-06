#include "stdafx.h"
#include "Enemy.h"

#include "PlayerCursor.h"

Enemy::Enemy()
	: m_health(5)

	, m_hitEffecter(nullptr)
{
}
Enemy::~Enemy()
{
}



void Enemy::Hit(const UINT & damage)
{
	if (m_health > damage)
	{
		m_health -= damage;
		m_hitEffecter->PlayerAttackHitToEnemy();
		GetSingleton(SoundManager)->Play(L"./Resource/sound/Hit_Sound.wav");
		//PlaySoundW(L"./Resource/sound/Hit_Sound.wav", NULL, SND_FILENAME | SND_ASYNC);
		//PlaySoundW(L"./Resource/sound/Kill_Sound.wav", NULL, SND_FILENAME | SND_ASYNC);
		//sndPlaySoundW(L"./Resource/sound/Hit_Sound.wav",  SND_FILENAME |SND_ASYNC);
		//sndPlaySoundW(L"./Resource/sound/Kill_Sound.wav",  SND_FILENAME |SND_ASYNC);
	}
	else
	{
		m_health = 0;
		m_hitEffecter->PlayerAttackKillToEnemy();
		sndPlaySoundW(L"./Resource/sound/Kill_Sound.wav", SND_ASYNC | SND_NODEFAULT);
	}
}
