#pragma once
#include "EnemyAttack.h"
class EnemyDropshot :
	public EnemyAttack
{
private:
	D3DXVECTOR3 m_pos;
	UINT m_duration;

	std::vector<EnemyAttack*>* m_attackContainer;

public:
	void Update(Player* player) override;
	void Render() override;

public:
	EnemyDropshot(const D3DXVECTOR3& pos, std::vector<EnemyAttack*>* attackContainer);
	~EnemyDropshot();
};

