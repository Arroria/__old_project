#pragma once
namespace AF
{
	class MapManager;

	class Item;
	class ItemSender;

	class ASEModel;

	class ItemManager
	{
	private:
		MapManager**		m_mapManager;

		std::map<std::wstring, ASEModel*>	m_itemModel;

		std::list<Item*>		m_item;
		std::list<ItemSender*>	m_itemSender;


	public:
		void	CreateItem		(const int& type, const D3DXVECTOR3& itemDropPosition);
		void	CreateItemSender(const int& type, const D3DXVECTOR3& itemDropPosition);
		Item*	CheckGetItem	(const D3DXVECTOR3& playerPos, const int& playerSize);

	public:
		void	Update();
		void	Render();

	public:
		ItemManager(MapManager** mapManager);
		~ItemManager();
	};
}
