#pragma once
class Item;
class ItemSupplier
{
private:
	bool m_destroyThis;
public:
	bool CheckDestroyThis() { return m_destroyThis; }
	void DestroyThis() { m_destroyThis = true; }

private:
	D3DXVECTOR3 m_position;
	float m_rotation;
	D3DXVECTOR3 m_scale;

	int m_itemId;
	float m_landingHeight;

	std::list<Item*>* m_itemContainer;

public:
	void Update();
	void Render();

public:
	ItemSupplier(const D3DXVECTOR3& position, const int& itemId, const float& landingHeight, std::list<Item*>* itemContainer);
	~ItemSupplier();
};

