#pragma once
#include "Singleton.h"

class Scene
{
public:
	virtual void Initialize() PURE;
	virtual void Update() PURE;
	virtual void Render() PURE;
	virtual void Release() PURE;

public:
	Scene() {}
	~Scene() {}
};

class SceneManager :
	public Singleton<SceneManager>
{
private:
	std::map<std::wstring, Scene*> m_sceneContainer;
	Scene* m_activatedScene;

public:
	void RegisterScene(const std::wstring& key, Scene* scene);
	void ChangeScene(const std::wstring& key);

public:
	void Initialize();
	void Update();
	void Render();
	void Release();

public:
	SceneManager();
	~SceneManager();
};

