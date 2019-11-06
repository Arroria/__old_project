#pragma once
#include "Singleton.h"
class CScene;
class CSceneMgr :
	public CSingleton<CSceneMgr>
{
private:
	std::map<std::string, CScene*>	m_sceneContainer;
	CScene*							m_nowScene;
	CScene*							m_nextScene;

private:
	void	CreateScene(const std::string& key, CScene* scene);
	void	ChangeScene();

public:
	void	ChangeScene(const std::string& key);

public:
	HRESULT	Init();
	void	Update();
	void	Render();
	void	Release();

public:
	CSceneMgr();
	~CSceneMgr();
};

#define SceneMgr			SINGLETON_GET(CSceneMgr)
#define SceneMgr_Release	SINGLETON_RELEASE(CSceneMgr)
