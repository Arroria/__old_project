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
		m_activateScene->Update();
	m_isSceneChanged = false;
}

void SceneManager::Render()
{
	if (m_activateScene && !m_isSceneChanged)
		m_activateScene->Render();
}



void SceneManager::RegistScene(const std::wstring & key, Scene * scene)
{
	auto& findSceneIter = m_sceneList.find(key);
	if (findSceneIter != m_sceneList.end())
		return;

	m_sceneList.insert(std::make_pair( key, scene ));
}

void SceneManager::ChangeScene(const std::wstring & key)
{
	if (m_activateScene)
	{
		m_activateScene->Release();
		m_activateScene = nullptr;
	}

	auto& findSceneIter = m_sceneList.find(key);
	if (findSceneIter == m_sceneList.end())
		return;

	m_activateScene = findSceneIter->second;
	m_activateScene->Initialize();

	m_isSceneChanged = true;
}
