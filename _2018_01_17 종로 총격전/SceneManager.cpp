#include "stdafx.h"
#include "SceneManager.h"


SceneManager::SceneManager()
	: m_activatedScene(nullptr)
{
}
SceneManager::~SceneManager()
{
}



#include "Tilte.h"
#include "Intro.h"
#include "_1Stage.h"
#include "Story.h"
#include "_2Stage.h"
#include "Ending.h"

void SceneManager::Initialize()
{
	RegisterScene(L"Intro", new Intro());
	RegisterScene(L"Title", new Tilte());
	RegisterScene(L"Stage1", new _1Stage());
	RegisterScene(L"Story", new Story());
	RegisterScene(L"Stage2", new _2Stage());
	RegisterScene(L"Ending", new Ending());


	ChangeScene(L"Intro");
} 

void SceneManager::Update()
{
	if (m_activatedScene)
		m_activatedScene->Update();
}

void SceneManager::Render()
{
	if (m_activatedScene)
		m_activatedScene->Render();
}

void SceneManager::Release()
{
	if (m_activatedScene)
		m_activatedScene->Release();

	for (auto& iter : m_sceneContainer)
		SAFE_DELETE(iter.second);
}



void SceneManager::RegisterScene(const std::wstring & key, Scene * scene)
{
	m_sceneContainer.insert(std::make_pair(key, scene));
}

void SceneManager::ChangeScene(const std::wstring & key)
{
	if (m_activatedScene)
		m_activatedScene->Release();

	auto iter = m_sceneContainer.find(key);
	if (iter == m_sceneContainer.end())
		return;

	m_activatedScene = iter->second;
	m_activatedScene->Initialize();
}

