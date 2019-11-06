#pragma once
#include "Effect.h"
class GrenadeEffect :
	public Effect
{
private:
	D3DXVECTOR2 m_position;

	UINT m_animation;

public:
	void Initialize() override;
	void Update() override;
	void Render() override;
	void Release() override;

public:
	GrenadeEffect(const D3DXVECTOR2& position);
	~GrenadeEffect();
};

