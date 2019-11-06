#pragma once

class EnemyAttack;
class Weakness
{
protected:
	UINT m_health;

	D3DXVECTOR3 m_pos;
	D3DXQUATERNION m_rot;

public:
	void Hit() { if (m_health) m_health--; }
	UINT GetHealth() { return m_health; }

public:
	virtual void Initialize() PURE;
	virtual void Update(const D3DXVECTOR3& pos, const D3DXQUATERNION& rot, std::vector<EnemyAttack*>* attackContainer, const D3DXVECTOR3& playerPos) PURE;
	virtual void Render(const D3DXMATRIX& parentLocalM, const D3DXMATRIX& parentSRM, const D3DXMATRIX& parentTM, ASE* model) PURE;
	virtual void Release() PURE;

public:
	Weakness();
	virtual ~Weakness();
};

