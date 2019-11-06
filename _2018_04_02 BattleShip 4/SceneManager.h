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
	void	RegistScene(const std::wstring& key, Scene* scene);
	void	ChangeScene(const std::wstring& key);

	bool IsSceneChanged() { return m_isSceneChanged; }

public:
	void Update();
	void Render();

public:
	SceneManager();
	~SceneManager();
};

