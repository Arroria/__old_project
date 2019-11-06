#pragma once
class AI_Helicopter;
class Jet;

class OBBCollision;
class Vulkan;

class Kaga
{
private:
	ASE* m_model;

	D3DXVECTOR3 m_position;
	D3DXQUATERNION m_rotation;
	
	std::vector<AI_Helicopter*>* m_aiSpawnPoint;
	Jet* m_player;
	ASE* m_aiModel;

	UINT m_summonCoolTime;
	ASE* m_attackModel;
	std::vector<Vulkan*>* m_enemyAttack;

	OBBCollision* m_obb;

	UINT m_health;

public:
	OBBCollision& GetOBB() { return *m_obb; }
	D3DXVECTOR3 GetPosition() { return m_position; }

	UINT GetHelath() { return m_health; }
	void SetHelath(const UINT& health) { m_health = health; }
	void Attacked() { if (m_health) m_health--; }

	void Initialize(Jet* player, ASE* aiModel, ASE* model, const D3DXQUATERNION& data);
	void Update();
	void Render();
	void Release();

public:
	Kaga(std::vector<AI_Helicopter*>* aiSpawnPoint, std::vector<Vulkan*>* enemyAttack, ASE* attackModel);
	~Kaga();
};

