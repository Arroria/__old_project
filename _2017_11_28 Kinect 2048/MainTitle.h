#pragma once
#include "BaseScene.h"
class SettingUI;
class GameModeUI;
class MainTitle :
	public BaseScene
{
private:
	//Render
	LPD3DXFONT m_titleFont;
	LPD3DXFONT m_menuFont;

	RECT_ex m_startPosition;
	RECT_ex m_optionPosition;
	RECT_ex m_exitPosition;

	//Menu
	int m_seletedMenu;

	GameModeUI* m_gameModeUI;
	SettingUI* m_settingUI;

public:
	virtual void Activated()override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void Disabled() override;

public:
	MainTitle();
	virtual ~MainTitle();
};

