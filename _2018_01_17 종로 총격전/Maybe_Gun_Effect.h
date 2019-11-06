#pragma once
#include "Effect.h"
class Maybe_Gun_Effect :
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
	Maybe_Gun_Effect(const D3DXVECTOR2& position);
	~Maybe_Gun_Effect();
};

