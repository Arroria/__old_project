#include "MainHeader.h"
#include "ItemManager.h"

#include "Item.h"
#include "ItemSender.h"

#include "ASE.h"

#define ITEMSIZE 8

namespace AF
{
	ItemManager::ItemManager(MapManager ** mapManager)
		: m_mapManager(mapManager)
	{
		ASELoader aseLoader;

		m_itemModel.insert(std::make_pair( L"Bomb",			aseLoader.LoadAseFile(L"./Resource/Item/Bomb.ase") ));
		m_itemModel.insert(std::make_pair( L"Star",			aseLoader.LoadAseFile(L"./Resource/Item/Star.ase") ));
		m_itemModel.insert(std::make_pair( L"HealthBox",	aseLoader.LoadAseFile(L"./Resource/Item/HealthBox.ase") ));
		m_itemModel.insert(std::make_pair( L"Shield",		aseLoader.LoadAseFile(L"./Resource/Item/Shield.ase") ));
		m_itemModel.insert(std::make_pair( L"Money",		aseLoader.LoadAseFile(L"./Resource/Item/Money.ase") ));

		m_itemModel.insert(std::make_pair( L"ItemSender",	aseLoader.LoadAseFile(L"./Resource/Item/ItemDropper.ase") ));
	}
	ItemManager::~ItemManager()
	{
		for (auto& iter : m_item)
			SAFE_DELETE(iter);
		for (auto& iter : m_itemSender)
			SAFE_DELETE(iter);
	}



	void ItemManager::Update()
	{
		auto& iter = m_item.begin();
		while (iter != m_item.end())
		{
			(*iter)->Update();
			if ((*iter)->IsDelete())
			{
				SAFE_DELETE(*iter);
				iter = m_item.erase(iter);
			}
			else
				iter++;
		}
		auto& iter2 = m_itemSender.begin();
		while (iter2 != m_itemSender.end())
		{
			(*iter2)->Update();
			if ((*iter2)->IsDelete())
			{
				SAFE_DELETE(*iter2);
				iter2 = m_itemSender.erase(iter2);
			}
			else
				iter2++;
		}
	}

	void ItemManager::Render()
	{
		for (auto& iter : m_item)
			iter->Render();
		for (auto& iter : m_itemSender)
			iter->Render();
	}
	
	
	
	void ItemManager::CreateItem(const int & type, const D3DXVECTOR3& itemDropPosition)
	{
		ASEModel* itemModel = nullptr;
		switch (type)
		{
			case 1:	itemModel = m_itemModel[L"Bomb"		];	break;
			case 2:	itemModel = m_itemModel[L"Star"		];	break;
			case 3:	itemModel = m_itemModel[L"HealthBox"];	break;
			case 4:	itemModel = m_itemModel[L"Shield"	];	break;
			case 5:	itemModel = m_itemModel[L"Money"	];	break;
		}
		Item* item = new Item(type, itemModel);
		item->SetPosition(D3DXVECTOR3(itemDropPosition.x, 10, itemDropPosition.z));
		m_item.push_back(item);
	}
	
	void ItemManager::CreateItemSender(const int & type, const D3DXVECTOR3& itemDropPosition)
	{
		m_itemSender.push_back( new ItemSender(this, type, m_itemModel[L"ItemSender"], itemDropPosition) );
	}
	
	Item * ItemManager::CheckGetItem(const D3DXVECTOR3 & playerPos, const int & playerSize)
	{
		RECT item, player;
		SetRect(&player, playerPos.x - playerSize, playerPos.z - playerSize, playerPos.x + playerSize, playerPos.z + playerSize);

		for (auto& iter : m_item)
		{
			SetRect(&item, iter->GetPosition().x - ITEMSIZE, iter->GetPosition().z - ITEMSIZE, iter->GetPosition().x + ITEMSIZE, iter->GetPosition().z + ITEMSIZE);
			if (item.left	< player.right	&&
				item.right	> player.left	&&
				item.top	< player.bottom	&&
				item.bottom	> player.top)
				return iter;
		}

		return nullptr;
	}
}
									