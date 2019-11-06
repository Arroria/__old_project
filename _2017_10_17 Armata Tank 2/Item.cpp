#include "stdafx.h"
#include "Item.h"

#include "BaseTank.h"
#include "PlayerTank.h"

#include "ASE.h"

#define GameDeltaTime g_processManager->GetDeltaTime()

#define LostTime 10.
#define FlashingTime 2.

Item::Item(const int& itemCode, const D3DXVECTOR3& position)
	: m_destroyThis(false)

	, m_itemCode(itemCode)
	, m_txItem(nullptr)

	, m_position(position)
	, m_rotation(0)
	, m_scale(1, 1, 1)

	, m_lostTime(NULL)

	, m_collisionSize{ 0, 0 }
{
	std::wstring txKey;
	switch (m_itemCode)
	{
	case 0://Health
		txKey = L"Item_Health";
		m_aseItem = g_ase->GetAse(L"Item_Health");
		break;
	case 1://
		txKey = L"Item_Cannon";
		m_aseItem = g_ase->GetAse(L"Item_Cannon");
		break;
	case 2:
		txKey = L"Item_AutoCannon";
		m_aseItem = g_ase->GetAse(L"Item_AutoCannon");
		break;
	case 3:
		txKey = L"Item_Rocket";
		m_aseItem = g_ase->GetAse(L"Item_Rocket");
		break;
	default:
		break;
	}
	m_collisionSize.x = m_collisionSize.y = 1;
	m_txItem = g_textureContainer->GetTexture(txKey);
	m_scale *= 0.5;
}
Item::~Item()
{
}


void Item::Update()
{
	if (m_lostTime < LostTime)
		m_lostTime += GameDeltaTime;
	else
	{
		DestroyThis();
		return;
	}

	m_rotation += GameDeltaTime * 360;
}

void Item::Render()
{
	if (LostTime - FlashingTime > m_lostTime || (int)(m_lostTime * 10) % 2)
	{
		D3DXMATRIX a, b, c;
		D3DXMatrixTranslation(&a, m_position.x, m_position.z, m_position.y);
		D3DXMatrixRotationY(&c, -D3DXToRadian(m_rotation));
		D3DXMatrixScaling(&b, m_scale.x, m_scale.z, m_scale.y);
		m_aseItem->Render(b * c * a);
	}

	GetSingleton(TextureRenderer)->SetTransform(m_position, m_rotation, m_scale);
	D3DXCOLOR color(1, 1, 1, 1);
	if (LostTime - FlashingTime <= m_lostTime)
		color.a = ((int)(m_lostTime * 4) % 2 ? 1 : 0);
	GetSingleton(TextureRenderer)->DrawTexture(m_txItem, color);
}


void Item::EffectForPlayer(BaseTank * target)
{
	switch (m_itemCode)
	{
	case 0:
		target->Repair(100);
		break;
	case 1:
		((PlayerTank*)target)->ChangeWeapon(PlayerTankWeaponType::Cannon);
		break;
	case 2:
		((PlayerTank*)target)->ChangeWeapon(PlayerTankWeaponType::AutoCannon);
		break;
	case 3:
		((PlayerTank*)target)->ChangeWeapon(PlayerTankWeaponType::Rocket);
		break;
	}

	DestroyThis();
}