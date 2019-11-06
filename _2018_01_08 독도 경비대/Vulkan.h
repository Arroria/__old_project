#pragma once
class OBBCollision;
class Vulkan
{
private:
	ASE* m_model;

	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_direction;
	D3DXMATRIX m_rotation;

	float m_speed;
	float m_duraction;

	OBBCollision* m_obb;
	bool m_delete;
	
public:
	void SetDelete() { m_delete = true; }
	bool GetDelete() { return m_delete; }

	OBBCollision& GetOBB() { return *m_obb; }

public:
	void Initialize(ASE* model, const D3DXVECTOR3& position, const D3DXQUATERNION& rotation, const float& speed);
	void Update();
	void Render();
	void Release();

public:
	Vulkan();
	~Vulkan();
};

