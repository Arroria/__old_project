#pragma once
#include "Weakness.h"

class EnemyAttack;
class YamatoTurret :
	public Weakness
{
private:
	UINT m_attackCooltime;
	UINT m_attackDuration;

public:
	void Initialize() override;
	void Update(const D3DXVECTOR3& pos, const D3DXQUATERNION& rot, std::vector<EnemyAttack*>* attackContainer, const D3DXVECTOR3& playerPos) override;
	void Render(const D3DXMATRIX& parentLocalM, const D3DXMATRIX& parentSRM, const D3DXMATRIX& parentTM, ASE* model) override;
	void Release() override;

public:
	YamatoTurret(const D3DXVECTOR3& pos, const D3DXQUATERNION& rot);
	~YamatoTurret();
};

