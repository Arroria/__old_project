#include "stdafx.h"
#include "Map.h"

#include "ItemSupplier.h"

#include "BaseBlock.h"
#include "DefaultBlock.h"

//Old version map constructure
///Map::Map(const POINT & mapSize, const int * mapConstructure)
///	: m_mapSize(mapSize)
///{
///	m_mapBlock.resize(mapSize.x * mapSize.y, nullptr);
///	for (int ySize = 0; ySize < mapSize.y; ySize++)
///	{
///		for (int xSize = 0; xSize < mapSize.x; xSize++)
///		{
///			const int& blockID = mapConstructure[ySize * mapSize.x + xSize];
///			BaseBlock** blockPlace = &m_mapBlock[xSize + ySize * mapSize.x];
///			switch (blockID)
///			{
///			default:
///				if (blockID < 10)
///					*blockPlace = new DefaultBlock(D3DXVECTOR3((xSize + 0.5f) * BaseBlockScale * 160, (ySize + 0.5f) * BaseBlockScale * 160, 0), blockID);
///				else
///					*blockPlace = new x4Block(D3DXVECTOR3((xSize + 0.5f) * BaseBlockScale * 160, (ySize + 0.5f) * BaseBlockScale * 160, 0), blockID - 10, 0);
///				break;
///			}
///		}
///	}
///
///	m_txBlockBreak.push_back(g_textureContainer->GetTexture(L"BlockBreak0_x10"));
///	m_txBlockBreak.push_back(g_textureContainer->GetTexture(L"BlockBreak1_x10"));
///	m_txBlockBreak.push_back(g_textureContainer->GetTexture(L"BlockBreak2_x10"));
///	m_txBlockBreak.push_back(g_textureContainer->GetTexture(L"BlockBreak3_x10"));
///	m_txBlockBreak.push_back(g_textureContainer->GetTexture(L"BlockBreak4_x10"));
///	m_txBlockBreak.push_back(g_textureContainer->GetTexture(L"BlockBreak5_x10"));
///	m_txBlockBreak.push_back(g_textureContainer->GetTexture(L"BlockBreak6_x10"));
///	m_txBlockBreak.push_back(g_textureContainer->GetTexture(L"BlockBreak7_x10"));
///	m_txBlockBreak.push_back(g_textureContainer->GetTexture(L"BlockBreak8_x10"));
///	m_txBlockBreak.push_back(g_textureContainer->GetTexture(L"BlockBreak9_x10"));
///}
Map::Map(MapData * mapData, std::list<ItemSupplier*>* itemSupplierContainer, std::list<Item*>* itemContainer)
	: m_mapData(mapData)

	, m_mapSize(mapData->mapSize)

	, m_itemSummonTime(0)
	, m_itemSupplierContainer(itemSupplierContainer)
	, m_itemContainer(itemContainer)
{
	m_mapBlock.resize(m_mapSize.x * m_mapSize.y, nullptr);

	D3DXVECTOR3 blockScale(D3DXVECTOR3(1, 1, 1) * mapData->blockSize / mapData->tileSize);
	D3DXVECTOR2 blockCollisionSize;
	blockCollisionSize.x = blockCollisionSize.y = mapData->tileSize / 2;

	for (int ySize = 0; ySize < m_mapSize.y; ySize++)
	{
		for (int xSize = 0; xSize < m_mapSize.x; xSize++)
		{
			const int& blockID = mapData->mapConstruct[ySize * m_mapSize.x + xSize];
			const BlockData& blockData = mapData->blockData[blockID];

			
			m_mapBlock[xSize + ySize * m_mapSize.x] = new DefaultBlock(blockID, D3DXVECTOR3((xSize + 0.5f) * mapData->tileSize, (ySize + 0.5f) * mapData->tileSize, 0), blockScale, blockCollisionSize, blockData.Durability, mapData->texture[blockID], blockData.Breakable, blockData.ObjectMoveable);
		}
	}

	m_txBlockBreak.push_back(g_textureContainer->GetTexture(L"BlockBreak0_x10"));
	m_txBlockBreak.push_back(g_textureContainer->GetTexture(L"BlockBreak1_x10"));
	m_txBlockBreak.push_back(g_textureContainer->GetTexture(L"BlockBreak2_x10"));
	m_txBlockBreak.push_back(g_textureContainer->GetTexture(L"BlockBreak3_x10"));
	m_txBlockBreak.push_back(g_textureContainer->GetTexture(L"BlockBreak4_x10"));
	m_txBlockBreak.push_back(g_textureContainer->GetTexture(L"BlockBreak5_x10"));
	m_txBlockBreak.push_back(g_textureContainer->GetTexture(L"BlockBreak6_x10"));
	m_txBlockBreak.push_back(g_textureContainer->GetTexture(L"BlockBreak7_x10"));
	m_txBlockBreak.push_back(g_textureContainer->GetTexture(L"BlockBreak8_x10"));
	m_txBlockBreak.push_back(g_textureContainer->GetTexture(L"BlockBreak9_x10"));
}
Map::~Map()
{
	for (auto& iter : m_mapBlock)
		SAFE_DELETE(iter);
	m_mapBlock.clear();

	m_txBlockBreak.clear();

	SAFE_DELETE(m_mapData);
}


D3DXVECTOR3 Map::GetPlayerSpawnPoint()
{ return D3DXVECTOR3(m_mapData->playerSpawnPoint.x + 0.5f, m_mapData->playerSpawnPoint.y + 0.5f, 0) * m_mapData->tileSize; }
D3DXVECTOR3 Map::GetPositionForTile(const int& xPosForTile, const int& yPosForTile)
{ return D3DXVECTOR3(xPosForTile + 0.5f, yPosForTile + 0.5f, 0) * m_mapData->tileSize; }

bool Map::MapUpdate()
{
	for (auto& iter : m_mapBlock)
	{
		if (iter)
		{
			iter->Update();
			if (iter->CheckDestroyThis())
			{
				D3DXVECTOR3 blockPos(iter->GetPosition());
				auto dummy = iter;
				iter = new DefaultBlock(0, iter->GetPosition(), iter->GetScale(), iter->GetCollisionSize(), m_mapData->blockData[0].Durability, m_mapData->texture[m_mapData->blockData[0].textureIndex], m_mapData->blockData[0].Breakable, m_mapData->blockData[0].ObjectMoveable);
				delete dummy;
			}
		}
	}


	//Summmon itemSupplier
	#define ItemSummonTime 5
	m_itemSummonTime += g_processManager->GetDeltaTime();
	if (m_itemSummonTime >= ItemSummonTime)
	{
		int refindCheck(0);
		while (refindCheck < 10)
		{
			refindCheck++;

			BaseBlock* block = m_mapBlock[rand() % m_mapData->mapSize.x + (rand() % m_mapData->mapSize.y) * m_mapData->mapSize.x];
			if (block && block->IsCanPenetrateForMove())
			{
				m_itemSupplierContainer->push_back(new ItemSupplier(block->GetPosition() + D3DXVECTOR3(0, 0, m_mapData->tileSize * 10.5f), rand() % 4, m_mapData->tileSize, m_itemContainer));
				break;
			}
		}
		m_itemSummonTime -= ItemSummonTime;
	}

	return false;
}

bool Map::MapRender()
{
	for (auto& iter : m_mapBlock)
	{
		if (iter)
		{
			iter->Render();
			
			///float blockDurability(iter->GetDurability()), blockMaxDurability(iter->GetMaxDurability());
			///
			///if (blockDurability != blockMaxDurability && blockDurability != 0)
			///{
			///	D3DXVECTOR3 scale(D3DXVECTOR3(1,1,1) * 5);
			///	D3DXMATRIX mat1, mat2;
			///	D3DXMatrixTranslation(&mat2, iter->GetPosition().x, iter->GetPosition().z + 10, iter->GetPosition().y);
			///	D3DXMatrixScaling(&mat1, scale.x, scale.z, scale.y);
			///	g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(mat1 * mat2));
			///	g_processManager->GetDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
			///	g_processManager->GetDevice()->SetTexture(0, m_txBlockBreak[(1 - (blockDurability / blockMaxDurability)) * m_txBlockBreak.size()]->d3dxTexture);
			///	g_processManager->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
			///	struct temp
			///	{
			///		D3DXVECTOR3 p;
			///		D3DXVECTOR2 uv;
			///		temp(D3DXVECTOR3 _p, D3DXVECTOR2 _uv) : p(_p), uv(_uv) {}
			///	};
			///	std::vector<temp> tmp;
			///
			///	tmp.push_back(temp(D3DXVECTOR3(-1, 1.01, 1), D3DXVECTOR2(0, 0)));
			///	tmp.push_back(temp(D3DXVECTOR3(1, 1.01, 1), D3DXVECTOR2(1, 0)));
			///	tmp.push_back(temp(D3DXVECTOR3(1, 1.01, -1), D3DXVECTOR2(1, 1)));
			///	tmp.push_back(temp(D3DXVECTOR3(-1, 1.01, 1), D3DXVECTOR2(0, 0)));
			///	tmp.push_back(temp(D3DXVECTOR3(1, 1.01, -1), D3DXVECTOR2(1, 1)));
			///	tmp.push_back(temp(D3DXVECTOR3(-1, 1.01, -1), D3DXVECTOR2(0, 1)));
			///	g_processManager->GetDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			///	g_processManager->GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			///	g_processManager->GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
			///	g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &tmp[0], sizeof(temp));
			///
			///	//GetSingleton(TextureRenderer)->SetTransform(iter->GetPosition(), D3DXVECTOR3(BaseBlockScale, BaseBlockScale, BaseBlockScale));
			///	//GetSingleton(TextureRenderer)->DrawTexture(m_txBlockBreak[(1 - (blockDurability / blockMaxDurability)) * m_txBlockBreak.size()]);
			///}
		}
	}
	return false;
}



MapData* Map::CreateMapFromFile(const std::wstring & path)
{
	FILE* mapFile;
	_wfopen_s(&mapFile, path.c_str(), L"r");
	if (!mapFile)
		return nullptr;

	MapData* mapData = new MapData();

	std::wstring dataFromFile;
	auto GetData = [&mapFile, &dataFromFile]()->const std::wstring&
	{
			dataFromFile.clear();
			
			WCHAR data = NULL;
			bool isString(false);

			while (!feof(mapFile))
			{
				data = fgetwc(mapFile);

				//String Check
				if (data == '\"')
				{
					if (isString)
						break;
					isString = true;
					continue;
				}

				//Whitespace Check
				if (data <= 32)
				{
					if (isString)
						continue;
					if (!dataFromFile.empty())
						break;
					else
						continue;
				}

				dataFromFile += data;
			}
			return dataFromFile;
		};
	auto GetDataAsInteger = [&GetData]()->int { return _wtoi(GetData().c_str()); };
	auto GetDataAsFloat = [&GetData]()->float { return _wtof(GetData().c_str()); };

	while (true)
	{
		GetData();
		if (dataFromFile.empty())
			break;

		if (dataFromFile == L"*MapSize")
		{
			mapData->mapSize.x = GetDataAsInteger();
			mapData->mapSize.y = GetDataAsInteger();
		}
		else if (dataFromFile == L"*BlockSize")
			mapData->blockSize = GetDataAsFloat();
		else if (dataFromFile == L"*TileSize")
			mapData->tileSize = GetDataAsFloat();
		else if (dataFromFile == L"*PlayerSpawnPoint")
		{
			mapData->playerSpawnPoint.x = GetDataAsInteger();
			mapData->playerSpawnPoint.y = GetDataAsInteger();
		}
		else if (dataFromFile == L"*TextureList")
		{
			int level(0);
			do {
				GetData();
				if (dataFromFile == L"{") level += 1;
				else if (dataFromFile == L"}") level -= 1;

				else if (dataFromFile == L"*TextureListSize") mapData->texture.resize(GetDataAsInteger());
				else if (dataFromFile == L"*Texture")
				{
					int index(GetDataAsInteger());

					GetData();
					g_textureContainer->LoadTexture(dataFromFile, dataFromFile);
					mapData->texture[index] = g_textureContainer->GetTexture(dataFromFile);
				}
			} while (level > 0);
		}
		else if (dataFromFile == L"*MapBlockData")
		{
			int level(0);
			do {
				GetData();
				if (dataFromFile == L"{") level += 1;
				else if (dataFromFile == L"}") level -= 1;

				else if (dataFromFile == L"*BlockListSize") mapData->blockData.resize(GetDataAsInteger());
				else if (dataFromFile == L"*Block")
				{
					BlockData& blockData = mapData->blockData[GetDataAsInteger()];

					int _level(0);
					do {
						GetData();
						if (dataFromFile == L"{") _level += 1;
						else if (dataFromFile == L"}") _level -= 1;

						else if (dataFromFile == L"*Texture") blockData.textureIndex = GetDataAsInteger();
						else if (dataFromFile == L"*Breakable") blockData.Breakable = (GetDataAsInteger() != 0);
						else if (dataFromFile == L"*ObjectMoveable") blockData.ObjectMoveable = (GetDataAsInteger() != 0);
						else if (dataFromFile == L"*Durability") blockData.Durability = GetDataAsFloat();
					} while (_level > 0);
				}
			} while (level > 0);
		}
		else if (dataFromFile == L"*MapConstruct")
		{
			int _level(0);
			do {
				GetData();
				if (dataFromFile == L"{") _level += 1;
				else if (dataFromFile == L"}")
					_level -= 1;

				else
					mapData->mapConstruct.push_back(_wtoi(dataFromFile.c_str()));
			} while (_level > 0);
		}
	}

	fclose(mapFile);
	return mapData;
}
