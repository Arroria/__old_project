#pragma once

class BaseButton;
class SettingUI
{
private:
	static float m_boxMovementSpeed;
	static float m_scoreAnimationSpeed;

public:
	static float GetBoxMovementSpeed()	{ return m_boxMovementSpeed; }
	static float GetScoreAnimationSpeed()	{ return m_scoreAnimationSpeed; }


private:
	SettingUI** m_thisPointer;

	LPD3DXFONT m_font;

	BaseButton* m_close;
	BaseButton* m_boxSpeedButton;
	BaseButton* m_scoreSpeedButton;

public:
	void Update();
	void Render();

public:
	SettingUI(SettingUI** thisPointer);
	~SettingUI();
};

