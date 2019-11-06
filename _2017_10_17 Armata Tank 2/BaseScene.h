#pragma once
class BaseScene
{
public:
	virtual bool Activated() PURE;
	virtual bool Update() PURE;
	virtual bool Render() PURE;
	virtual void Disabled() PURE;

public:
	BaseScene();
	virtual ~BaseScene();
};

