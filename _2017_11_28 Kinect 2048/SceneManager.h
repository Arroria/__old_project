#pragma once
#include "Singleton.h"
class BaseScene;
class SceneManager :
	public Singleton<SceneManager>
{
private:
	std::map<std::wstring, BaseScene*> m_sceneList;
	BaseScene* m_activateScene;
	
	bool m_ignoreRenderToNextFrame;

public:
	HRESULT SceneRegister(const std::wstring& key, BaseScene* scene);
	HRESULT ChangeScene(const std::wstring& key);

public:
	void	SceneUpdate();
	HRESULT SceneRender();

public:
	SceneManager();
	~SceneManager();
};

