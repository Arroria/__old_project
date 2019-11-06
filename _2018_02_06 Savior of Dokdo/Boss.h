#pragma once

class Weakness;
class EnemyAttack;

class Boss
{
protected:
	D3DXVECTOR3 m_pos;
	D3DXQUATERNION m_rot;

	std::vector<Weakness*> m_weakness;

public:
	virtual bool IsDead() PURE;

public:
	virtual void Initialize() PURE;
	virtual void Update(std::vector<EnemyAttack*>* attackContainer, const D3DXVECTOR3& playerPos) PURE;
	virtual void Render() PURE;
	virtual void Release() PURE;

public:
	Boss();
	~Boss();
};
