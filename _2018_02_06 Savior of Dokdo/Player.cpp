#include "stdafx.h"
#include "Player.h"

#include "Frame.h"
#include "EnemyJet.h"



Player::Player()
	: m_attackCooltime(0)
	, m_health(100)

	, m_isFired(false)
	, m_ammoFirePos(0, 0, 0)

	, m_cameraShake(0)
{
}
Player::~Player()
{
}
