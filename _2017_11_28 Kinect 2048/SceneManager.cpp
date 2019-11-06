#include "stdafx.h"
#include "SceneManager.h"
#include "BaseScene.h"


#include "InGame.h"
#include "MainTitle.h"


SceneManager::SceneManager()
	: m_activateScene(nullptr)
	, m_ignoreRenderToNextFrame(false)
{
}
SceneManager::~SceneManager()
{
	if (m_activateScene)
		m_activateScene->Disabled();
	for (auto& sceneIter : m_sceneList)
		SAFE_DELETE(sceneIter.second);
	m_sceneList.clear();
}


void SceneManager::SceneUpdate()
{
	if (m_activateScene)
		m_activateScene->Update();
}

HRESULT SceneManager::SceneRender()
{
	if (m_ignoreRenderToNextFrame)
	{
		m_ignoreRenderToNextFrame = false;
		return E_FAIL;
	}

	if (m_activateScene)
		m_activateScene->Render();
	return S_OK;
}

HRESULT SceneManager::SceneRegister(const std::wstring & key, BaseScene * scene)
{
	if (!scene || m_sceneList.find(key) != m_sceneList.end())
		return E_FAIL;

	m_sceneList.insert(std::make_pair(key, scene));
	return S_OK;
}

HRESULT SceneManager::ChangeScene(const std::wstring & key)
{
	if (m_activateScene)
		m_activateScene->Disabled();
	
	auto& sceneIter = m_sceneList.find(key);
	if (sceneIter == m_sceneList.end())
	{
		m_activateScene = nullptr;
		return E_FAIL;
	}
	else
	{
		m_activateScene = sceneIter->second;
		m_activateScene->Activated();
	}

	return S_OK;
}
