#pragma once
#include "Enemy.h"


class Frame;
class Jet;

class EnemyCannon;

class EnemyJet
	: public Enemy
{
private:
	ASE* m_model;
	Jet* m_jet;

	UINT m_pattern;
	UINT m_patternDuration;
	bool m_rollToRight;

	UINT m_attackCooltime;

	Texture* m_texHealthFrame;

public:
	Frame* GetFrame() override;

private:
	void AIPattern_Roll(const D3DXVECTOR3& playerPos);
	void AIPattern_GoForward(const D3DXVECTOR3& playerPos);
	void AIPattern_FollowPlayer(const D3DXVECTOR3& playerPos);

	void Attack(std::vector<EnemyAttack*>* attackContainer, const D3DXVECTOR3& playerPos);

public:
	void Initialize(const D3DXVECTOR3& pos) override;
	void Update(std::vector<EnemyAttack*>* attackContainer, const D3DXVECTOR3& playerPos) override;
	void Render() override;
	void Release() override;

public:
	EnemyJet(ASE* model);
	~EnemyJet();
};

