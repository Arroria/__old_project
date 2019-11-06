#pragma once
#include "Frame.h"

class Ship
	: public Frame
{
private:
	D3DXQUATERNION m_objectiveRollRotation;
	D3DXQUATERNION m_objectiveYawRotation;

	float m_objectiveSpeed;

public:
	void Roll(const INT& rollToRight);
	void Accelerate(const INT& accelerate);

public:
	void Initialize();
	void Update(const INT& accelerate, const INT& rollToRight);
	void Render(LPDIRECT3DDEVICE9 device, ASE* model);
	void Release();

public:
	Ship();
	~Ship();
};

