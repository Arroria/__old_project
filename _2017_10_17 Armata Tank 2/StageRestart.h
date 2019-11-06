#pragma once
#include "BaseScene.h"
class StageRestart :
	public BaseScene
{
public:
	virtual bool Activated() override;
	virtual bool Update() override;
	virtual bool Render() override;
	virtual void Disabled() override;

public:
	StageRestart();
	~StageRestart();
};

