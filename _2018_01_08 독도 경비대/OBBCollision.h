#pragma once
class OBBCollision
{
	friend OBBCollision;

private:
	D3DXVECTOR3 m_centerPosition;
	D3DXVECTOR3 m_size;
	D3DXQUATERNION m_rotation;

public:
	D3DXVECTOR3 GetCenterPosition() const { return m_centerPosition; }
	D3DXVECTOR3 GetSize() const { return m_size; }
	D3DXQUATERNION GetRotation() const { return m_rotation; }

public:
	void Update(const D3DXVECTOR3* centerPosition, const D3DXVECTOR3* size, const D3DXQUATERNION* rotation);
	void Render(LPDIRECT3DDEVICE9 device, const D3DXCOLOR& color);
	bool CalculateCollision(const OBBCollision& anotherOBB);

public:
	OBBCollision();
	~OBBCollision();
};

