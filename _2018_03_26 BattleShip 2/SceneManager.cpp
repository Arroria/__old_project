#include "DXUT.h"
#include "SceneManager.h"


SceneManager::SceneManager()
	: m_activateScene(nullptr)
	, m_sceneChanged(false)
{
}
SceneManager::~SceneManager()
{
	if (m_activateScene)
		m_activateScene->Release();

	for (auto& scene : m_sceneList)
		SAFE_DELETE( scene.second );
	m_sceneList.clear();
}



void SceneManager::Update()
{
	if (m_activateScene)
		m_activateScene->Update();
	m_sceneChanged = false;
}

void SceneManager::Render()
{
	if (m_activateScene && !m_sceneChanged)
		m_activateScene->Render();
}



Scene* SceneManager::RegistScene(const std::wstring & key, Scene * scene)
{
	{
		auto& scene = m_sceneList.find(key);
		if (scene != m_sceneList.end())
			return scene->second;
	}

	m_sceneList.insert(std::make_pair( key, scene ));
	return scene;
}

Scene* SceneManager::ChangeScene(const std::wstring & key)
{
	if (m_activateScene)
		m_activateScene->Release();

	auto& scene = m_sceneList.find(key);
	if (scene == m_sceneList.end())
		return nullptr;

	m_activateScene = scene->second;
	if (m_activateScene)
		m_activateScene->Initialize();
	m_sceneChanged = true;
	return scene->second;
}
