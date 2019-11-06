#pragma once

class BaseButton;
class GameModeUI
{
private:
	static int m_gameSize[2];
	static bool m_isHardestMode;

public:
	static int*	GetGameSize() { return m_gameSize; }
	static bool	IsHardestMode() { return m_isHardestMode; }

private:
	//This
	GameModeUI** m_thisPointer;

	//Button
	BaseButton* m_btModeLite;
	BaseButton* m_btModeHardest;
	BaseButton* m_btModeExtend;
	BaseButton* m_btModeExtendSizingX;
	BaseButton* m_btModeExtendSizingY;

	BaseButton* m_btStart;
	BaseButton* m_btClose;
	//ÆóÁö, Á¹¶ó¾î·Á¿ò
	///	BaseButton* m_btModeDual;

	//Selete
	int m_selete;

	//Font
	LPD3DXFONT m_font;

public:
	void Update();
	void Render();

public:
	GameModeUI(GameModeUI** thisPointer);
	~GameModeUI();
};

