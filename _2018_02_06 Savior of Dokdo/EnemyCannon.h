#pragma once
#include "EnemyAttack.h"

class EnemyCannon
	: public EnemyAttack
{
private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_dir;
	D3DXQUATERNION m_rotQ;

	UINT m_duration;

public:
	void Update(Player* player) override;
	void Render() override;

public:
	EnemyCannon(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir);
	~EnemyCannon();
};

