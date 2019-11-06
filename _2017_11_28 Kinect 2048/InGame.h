#pragma once
#include "BaseScene.h"

class BaseButton;

class GameWorld;
class SettingUI;
class InGame :
	public BaseScene
{
private:
	//Game
	GameWorld* m_gameWorld;
	SettingUI* m_settingUI;

	//UI Font
	LPD3DXFONT m_titleFont;
	LPD3DXFONT m_scoreFont;
	LPD3DXFONT m_menuFont;

	//ScoreAnimation
	__int64 m_prevSocre;
	__int64 m_nowScore;
	__int64 m_objectiveScore;
	double m_elapsedTime;

	//Button
	BaseButton* m_settingUICallButton;
	BaseButton* m_goMain;

public:
	virtual void Activated()override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void Disabled() override;

public:
	InGame();
	~InGame();
};

