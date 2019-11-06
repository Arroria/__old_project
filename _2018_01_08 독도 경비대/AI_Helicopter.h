#pragma once
class Helicopter;
class Jet;
class OBBCollision;
class Vulkan;
class AI_Helicopter
{
private:
	Helicopter* m_me;

	Jet* m_temp;

	int m_tempHealth;

	ASE* m_attackModel;
	UINT m_attackCooltime;
	std::vector<Vulkan*>* m_enemyAttack;

	bool m_delete;

public:
	void SetPosition(const D3DXVECTOR3& position);
	void SetRotation(const D3DXQUATERNION& rotation);
	void HealthDown() { m_tempHealth--; }

	OBBCollision& GetOBB();
	bool SetDelete() { m_delete = true; }
	bool GetDelete() { return m_delete; }

public:
	void Initialize(ASE* bodyModel, ASE* topRotorModel, ASE* tailRotorModel);
	void Update();
	void Render();
	void Release();

public:
	AI_Helicopter(Jet* temp, std::vector<Vulkan*>* enemyAttack, ASE* attackModel);
	~AI_Helicopter();
};

