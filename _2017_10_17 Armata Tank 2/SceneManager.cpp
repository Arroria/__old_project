#include "stdafx.h"
#include "SceneManager.h"
#include "BaseScene.h"


SceneManager::SceneManager()
	: m_activatedScene(nullptr)
	, m_ignoreSceneRender(true)
{
}
SceneManager::~SceneManager()
{
	if (m_activatedScene)
		m_activatedScene->Disabled();

	for (auto& iter : m_sceneContainer)
	{
		SAFE_DELETE(iter.second);
	}
	m_sceneContainer.clear();
}


bool SceneManager::RegisterScene(const std::wstring & key, BaseScene * scene)
{
	//세부 미구현
	if (m_sceneContainer.find(key) != m_sceneContainer.end())
		return true;

	if (!scene)
		return true;

	m_sceneContainer.insert(std::make_pair(key, scene));
	return false;
}

bool SceneManager::ChangeScene(const std::wstring & key)
{
	m_ignoreSceneRender = true;

	if (m_activatedScene)
	{
		m_activatedScene->Disabled();
		m_activatedScene = nullptr;
	}

	auto nextSceneIter = m_sceneContainer.find(key);
	if (nextSceneIter == m_sceneContainer.end())
		return true;

	BaseScene* nextScene = nextSceneIter->second;
	if (nextScene->Activated())
		return true;

	m_activatedScene = nextScene;
	return false;
}

bool SceneManager::SceneUpdate()
{
	if (m_activatedScene)
		return m_activatedScene->Update();
	return true;
}

bool SceneManager::SceneRender()
{
	if (m_activatedScene)
	{
		if (m_ignoreSceneRender)
		{
			m_ignoreSceneRender = false;
			return false;
		}
		return m_activatedScene->Render();
	}
	return true;
}
