#pragma once
#include "Effect.h"
class EnemyDead :
	public Effect
{
private:
	D3DXVECTOR2 m_position;
	bool m_lookAtRight;

	UINT m_animation;
	UINT m_duration;
	
public:
	void Initialize() override;
	void Update() override;
	void Render() override;
	void Release() override;

public:
	EnemyDead(const D3DXVECTOR2& position, const bool& lookAtRight);
	~EnemyDead();
};

