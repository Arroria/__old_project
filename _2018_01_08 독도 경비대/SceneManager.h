#pragma once
#include "Singleton.h"


class BaseScene;
class SceneManager
	: public Singleton<SceneManager>
{
private:
	std::map<std::wstring, BaseScene*> m_sceneContainer;
	BaseScene* m_activateScene;

public:
	void CreateScene(const std::wstring& key, BaseScene* scene);
	void ChangeScene(const std::wstring & key);

public:
	void Initailize();
	void Update();
	void Render();
	void Release();

public:
	SceneManager();
	~SceneManager();
};

class BaseScene
{
public:
	virtual void Initialize() PURE;
	virtual void Update() PURE;
	virtual void Render() PURE;
	virtual void Release() PURE;

	BaseScene() {}
	virtual ~BaseScene() {}
};
