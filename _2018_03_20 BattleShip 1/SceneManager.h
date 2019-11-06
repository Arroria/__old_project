#pragma once
#include "Singleton.h"

__interface Scene
{
public:
	virtual void Initialize () PURE;
	virtual void Update() PURE;
	virtual void Render() PURE;
	virtual void Release() PURE;
};

class SceneManager :
	public Singleton<SceneManager>
{
private:
	std::map<std::wstring, Scene*> m_sceneList;
	Scene* m_activateScene;
	bool m_sceneChanged;

public:
	void RegisterScene(Scene* scene, const std::wstring& key);
	void ChangeScene(const std::wstring& key);

	bool SceneChanged() { return m_sceneChanged; }

public:
	void Update();
	void Render();

public:
	SceneManager();
	~SceneManager();
};

