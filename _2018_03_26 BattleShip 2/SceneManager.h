#pragma once
#include "Singleton.h"

#include "Scene.h"

class SceneManager :
	public Singleton<SceneManager>
{
private:
	std::map<std::wstring, Scene*> m_sceneList;
	Scene* m_activateScene;
	bool m_sceneChanged;

public:
	bool IsSceneChanged() { return m_sceneChanged; }
	Scene* RegistScene(const std::wstring& key, Scene* scene);
	Scene* ChangeScene(const std::wstring& key);

public:
	void Update();
	void Render();

public:
	SceneManager();
	~SceneManager();
};

