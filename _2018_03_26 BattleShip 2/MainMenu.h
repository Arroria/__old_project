#pragma once
#include "Scene.h"
enum SceneState;

class MainMenu :
	public Scene
{
private:
	bool m_first;
	SceneState m_sceneState;
	SceneState m_nextState;

	std::pair<UINT, UINT> m_frame;
	std::pair<UINT, bool> m_select;

	bool m_introStop;

	Channel* m_bgm;

private:
	bool BG_Update();

public:
	void Initialize() override;
	void Update() override;
	void Render() override;
	void Release() override;

public:
	MainMenu();
	~MainMenu();
};

