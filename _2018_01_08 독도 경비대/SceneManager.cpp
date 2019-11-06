#include "stdafx.h"
#include "SceneManager.h"


SceneManager::SceneManager()
{
}
SceneManager::~SceneManager()
{
}

#include "Main.h"
#include "AirWar.h"

void SceneManager::Initailize()
{
	CreateScene(L"Title", new Main());
	CreateScene(L"2Stage", new AirWar());

	m_activateScene = m_sceneContainer[L"Title"];
	m_activateScene->Initialize();
}

void SceneManager::Update()
{
	if (m_activateScene)
		m_activateScene->Update();
}

void SceneManager::Render()
{
	if (m_activateScene)
		m_activateScene->Render();
}

void SceneManager::Release()
{
	m_activateScene->Release();
	for (auto& iter : m_sceneContainer)
		SAFE_DELETE(iter.second)
}



void SceneManager::CreateScene(const std::wstring & key, BaseScene * scene)
{
	m_sceneContainer.insert(std::make_pair(key, scene));
}

void SceneManager::ChangeScene(const std::wstring & key)
{
	m_activateScene->Release();
	m_activateScene = m_sceneContainer[key];
	m_activateScene->Initialize();
}
