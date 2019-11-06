#pragma once
#include "Singleton.h"

class BaseScene;

class SceneManager
	: public Singleton<SceneManager>
{
private:
	std::map<std::wstring, BaseScene*> m_sceneContainer;
	BaseScene* m_activatedScene;

	bool m_ignoreSceneRender;

public:
	bool RegisterScene(const std::wstring& key, BaseScene* scene);

	bool ChangeScene(const std::wstring& key);

	bool SceneUpdate();
	bool SceneRender();

	//void UnregisterScene(const std::wstring& key); 구현 안할꺼임

public:
	SceneManager();
	~SceneManager();
};

