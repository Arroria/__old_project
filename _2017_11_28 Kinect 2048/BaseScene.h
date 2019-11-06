#pragma once
class BaseScene
{
public:
	virtual void Activated() PURE;
	virtual void Update() PURE;
	virtual void Render() PURE;
	virtual void Disabled() PURE;

public:
	BaseScene();
	virtual ~BaseScene();
};

