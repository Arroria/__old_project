#include "stdafx.h"
#include "SceneMgr.h"
#include "Scene.h"

CSceneMgr::CSceneMgr()
	: m_nowScene(nullptr)
	, m_nextScene(nullptr)
{
}
CSceneMgr::~CSceneMgr()
{
}



#include "ScMainMenu.h"
#include "ScCredit.h"
#include "ScInGame.h"

HRESULT CSceneMgr::Init()
{
	CreateScene("MainMenu",	new ScMainMenu());
	CreateScene("Credit",	new ScCredit());
	CreateScene("InGame",	new ScInGame());

	ChangeScene("MainMenu");
	return S_OK;
}

void CSceneMgr::Update()
{
	ChangeScene();
	if (m_nowScene)
		m_nowScene->Update();
}

void CSceneMgr::Render()
{
	if (m_nowScene)
		m_nowScene->Render();
}

void CSceneMgr::Release()
{
	m_nowScene->Release();

	for (auto& iter : m_sceneContainer)
		SAFE_DELETE(iter.second);
}



void CSceneMgr::CreateScene(const std::string & key, CScene * scene)
{
	m_sceneContainer.insert(std::make_pair(key, scene));
}

void CSceneMgr::ChangeScene()
{
	if (m_nextScene)
	{
		if (m_nowScene)
			m_nowScene->Release();

		m_nextScene->Init();

		m_nowScene = m_nextScene;
		m_nextScene = nullptr;
	}
}

void CSceneMgr::ChangeScene(const std::string & key)
{
	m_nextScene = m_sceneContainer.find(key)->second;
}
