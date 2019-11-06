#pragma once
#include "BaseScene.h"
class Tilte :
	public BaseScene
{
private:
	Texture* m_txMainScreen;

public:
	virtual bool Activated() override;
	virtual bool Update() override;
	virtual bool Render() override;
	virtual void Disabled() override;

public:
	Tilte();
	~Tilte();
};

