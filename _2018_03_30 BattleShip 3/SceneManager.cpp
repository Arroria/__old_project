#include "DXUT.h"
#include "SceneManager.h"

#include "Scene.h"


SceneManager::SceneManager()
	: m_activateScene(nullptr)
	, m_isSceneChanged(false)
{
}
SceneManager::~SceneManager()
{
	if (m_activateScene)
		m_activateScene->Release();

	for (auto& scene : m_sceneList)
		SAFE_DELETE(scene.second);
	m_sceneList.clear();
}



void SceneManager::Update()
{
	if (m_activateScene)
	{
		m_activateScene->Update();
		m_isSceneChanged = false;
	}
}

void SceneManager::Render()
{
	if (!m_isSceneChanged && m_activateScene)
		m_activateScene->Render();
}



void SceneManager::RegistScene(const std::wstring & key, Scene * scene)
{
	m_sceneList.insert(std::make_pair( key, scene ));
}

Scene * SceneManager::SceneChange(const std::wstring & key)
{
	m_isSceneChanged = true;
	
	if (m_activateScene)
		m_activateScene->Release();

	auto& sceneIter = m_sceneList.find(key);
	if (sceneIter == m_sceneList.end())
		return nullptr;

	m_activateScene = sceneIter->second;
	m_activateScene->Initialize();
	return nullptr;
}
