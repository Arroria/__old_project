#pragma once
#include "BaseScene.h"
class BaseTank;
class BaseShell;
class BaseBlock;
class Map;
class Item;
class ItemSupplier;

class PlayerAdmin;
class ShellAdmin;
class EnemyTankAdmin;

class Stage :
	public BaseScene
{
private:
	float m_cameraRotation;
	float m_cameraRange;

private:
	PlayerAdmin* m_playerAdmin;
	ShellAdmin* m_shellAdmin;
	EnemyTankAdmin* m_enemyAdmin;

	Map* m_testMap;
	std::list<Item*> m_itemContainer;
	std::list<ItemSupplier*> m_itemSupplierContainer;
	
public:
	virtual bool Activated() override;
	virtual bool Update() override;
	virtual bool Render() override;
	virtual void Disabled() override;

public:
	Stage();
	virtual ~Stage();
};

