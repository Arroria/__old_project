#include "DXUT.h"
#include "SceneManager.h"



SceneManager::SceneManager()
	: m_activateScene(nullptr)
	, m_sceneChanged(true)
{
}
SceneManager::~SceneManager()
{
	m_activateScene->Release();
	for (auto& iter : m_sceneList)
		SAFE_DELETE( iter.second );
	m_sceneList.clear();
}



void SceneManager::Update()
{
	if (m_activateScene)
	{
		m_sceneChanged = false;
		m_activateScene->Update();
	}
}

void SceneManager::Render()
{
	if (m_activateScene && !m_sceneChanged)
		m_activateScene->Render();
}



void SceneManager::RegisterScene(Scene * scene, const std::wstring & key)
{
	m_sceneList.insert(std::make_pair(key, scene));
}

void SceneManager::ChangeScene(const std::wstring & key)
{
	if (m_activateScene)
		m_activateScene->Release();

	auto& iter = m_sceneList.find(key);
	if (iter == m_sceneList.end())
	{
		assert("ERROR");
		return;
	}
	m_activateScene = iter->second;
	m_activateScene->Initialize();

	m_sceneChanged = true;
}