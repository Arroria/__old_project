#pragma once
class Scene
{
public:
	virtual void Initialize() PURE;
	virtual void Update() PURE;
	virtual void Render() PURE;
	virtual void Release() PURE;

public:
	Scene();
	virtual ~Scene();
};

