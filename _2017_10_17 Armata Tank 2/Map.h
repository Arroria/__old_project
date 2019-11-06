#pragma once
class ItemSupplier;
class Item;


struct MapData;

class BaseBlock;
class Map
{
public:
	static MapData* CreateMapFromFile(const std::wstring& path);

//Just Map
private:
	MapData* m_mapData;

	float m_defaultBlockSize;
	POINT m_mapSize;
	std::vector<BaseBlock*> m_mapBlock;

	std::vector<Texture*> m_txBlockBreak;


	double m_itemSummonTime;
	std::list<ItemSupplier*>* m_itemSupplierContainer;
	std::list<Item*>* m_itemContainer;

public:
	std::vector<BaseBlock*>* GetMapBlockContainer() { return &m_mapBlock; }

//Interaction data
public:
	D3DXVECTOR3 GetPlayerSpawnPoint();
	D3DXVECTOR3 GetPositionForTile(const POINT& positionForTile) { GetPositionForTile(positionForTile.x, positionForTile.y); }
	D3DXVECTOR3 GetPositionForTile(const int& xPosForTile, const int& yPosForTile);

//U/R
public:
	bool MapUpdate();
	bool MapRender();

public:
	//Map(const POINT& mapSize, const int* mapConstructure);
	Map(MapData* mapData, std::list<ItemSupplier*>* itemSupplierContainer, std::list<Item*>* itemContainer);
	~Map();
};

struct BlockData
{
	int textureIndex;
	
	bool Breakable;
	bool ObjectMoveable;
	
	float Durability;
	BlockData() {}
	~BlockData() {}
};

struct MapData
{
	POINT mapSize;
	float blockSize;
	float tileSize;

	POINT playerSpawnPoint;

	std::vector<Texture*> texture;
	std::vector<BlockData> blockData;

	std::vector<int> mapConstruct;

	MapData() {}
	~MapData() {}
};