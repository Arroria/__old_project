#pragma once
#include "Scene.h"
class ScMainMenu :
	public CScene
{
private:
	int isCrashed_start;
	int isCrashed_crdit;
	int isCrashed_exit;

	bool isPressed_start;
	bool isPressed_credit;
	bool isPressed_exit;

private:
	CBitmapImage*	UI_Main_Start;
	CBitmapImage*	UI_Main_Start_Click;
	CBitmapImage*	UI_Main_Start_Clicked;
	CBitmapImage*	UI_Main_Credit;
	CBitmapImage*	UI_Main_Credit_Click;
	CBitmapImage*	UI_Main_Credit_Clicked;
	CBitmapImage*	UI_Main_Exit;
	CBitmapImage*	UI_Main_Exit_Click;

public:
	virtual	HRESULT	Init()		override;
	virtual	void	Update()	override;
	virtual	void	Render()	override;
	virtual	void	Release()	override;
public:
	VECTOR2 Start;
	VECTOR2 Credit;
	VECTOR2 Exit;

public:
	ScMainMenu();
	~ScMainMenu();
};

