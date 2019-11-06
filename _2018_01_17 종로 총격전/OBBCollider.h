#pragma once
class OBBCollider
{
private:
	D3DXVECTOR2 m_position;
	D3DXVECTOR2 m_size;
	D3DXQUATERNION m_rotation;

public:
	D3DXVECTOR2		GetPosition()	const { return m_position; }
	D3DXVECTOR2		GetSize()		const { return m_size; }
	D3DXQUATERNION	GetRotation()	const { return m_rotation; }

public:
	void SetTransform(const D3DXVECTOR2* position, const D3DXVECTOR2* size, const D3DXQUATERNION* rotation);
	bool CalculateCollision(const OBBCollider& anotherOBB);
	void RenderBox(LPDIRECT3DDEVICE9 device);

public:
	OBBCollider();
	~OBBCollider();
};

