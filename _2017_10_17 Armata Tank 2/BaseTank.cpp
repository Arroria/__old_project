#include "stdafx.h"
#include "BaseTank.h"



BaseTank::BaseTank(const D3DXVECTOR3& position)
	: BaseTank(position, 1, 0)
{
}
BaseTank::BaseTank(const D3DXVECTOR3 & position, const float & durability, const float & armour)
	: m_destroyThis(false)


	, m_position(position)
	, m_rotation(0)
	, m_scale(1, 1, 1)

	, m_maxDurability(durability)
	, m_durability(durability)
	, m_armour(armour)

	, m_collisionSize(NULL, NULL)
{
}
BaseTank::~BaseTank()
{
}

bool BaseTank::Update()
{
	if (m_durability > 0)
	{
		Move();
		Aiming();
		Attack();
	}
	else
		DestroyThis();

	return false;
}


void BaseTank::Hit(const float & firePower, const float & penetration)
{
	///float temp = 0;
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
		///temp = damageCoefficient;
	}

	if (damage < 1)
		damage = 1;
	
	m_durability -= damage;
	if (m_durability < 0)
		m_durability = 0;
	///cout << "데미지 : " << damage << "\t데미지 비율 : " << temp << "\t체력 : " << m_durability << endl;
}

void BaseTank::Repair(const int & repairAmount)
{
	m_durability += repairAmount;
	if (m_durability > m_maxDurability)
		m_durability = m_maxDurability;
}
