#include "stdafx.h"
#include "BaseBlock.h"


BaseBlock::BaseBlock(const int& blockId, const D3DXVECTOR3& position, const D3DXVECTOR3& scale, const D3DXVECTOR2& collisionSize, const float & durability, const float & armour)
	: m_destroyThis(false)
	
	, m_blockId(blockId)

	, m_position(position)
	, m_scale(scale)

	, m_unbreakable(true)
	, m_maxDurability(durability)
	, m_durability(durability)
	, m_armour(armour)

	, m_collisionSize(collisionSize)
	, m_canPenetrateForMove(false)
	, m_canPenetrateForAttack(false)

{
	///float temp = BaseBlockScale * 80;
	///SetRect(&m_collisionBox, -temp, -temp, temp, temp);
}
BaseBlock::~BaseBlock()
{
}

void BaseBlock::Hit(const float & firePower, const float & penetration)
{
	if (m_unbreakable)
		return;

	float damage = NULL;
	float penetrationCoefficient = penetration / m_armour;

	if (penetrationCoefficient >= 1)
		damage = firePower;
	else if (penetrationCoefficient <= 0.5f)
		damage = (firePower > 1 ? 1 : firePower);
	else
	{
		float damageCoefficient = sinf(penetrationCoefficient * 2 - 1);
		damage = damageCoefficient * firePower;
	}

	if (damage < 1)
		damage = 1;

	m_durability -= damage;
	if (m_durability < 0)
		m_durability = 0;
}
