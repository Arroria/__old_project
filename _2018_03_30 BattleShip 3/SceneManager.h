#pragma once
#include "Singleton.h"

class Scene;
class SceneManager :
	public Singleton<SceneManager>
{
private:
	std::map<std::wstring, Scene*> m_sceneList;
	Scene* m_activateScene;
	bool m_isSceneChanged;

public:
	bool IsSceneChanged() { return m_isSceneChanged; }

	void RegistScene(const std::wstring& key, Scene* scene);
	Scene* SceneChange(const std::wstring& key);

public:
	void Update();
	void Render();

public:
	SceneManager();
	~SceneManager();
};

