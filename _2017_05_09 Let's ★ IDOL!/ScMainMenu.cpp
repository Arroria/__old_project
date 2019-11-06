#include "stdafx.h"
#include "ScMainMenu.h"


ScMainMenu::ScMainMenu()
{
}
ScMainMenu::~ScMainMenu()
{
}



HRESULT ScMainMenu::Init()
{
	//메인배경 받아오기
	m_sceneBackGroundImage = ResourceMgr->GetImage("BG_MainMenu");


	//UI 받아오기
	UI_Main_Start = ResourceMgr->GetImage("UI_Main_Start");
	UI_Main_Start_Click = ResourceMgr->GetImage("UI_Main_Start_Click");
	UI_Main_Start_Clicked = ResourceMgr->GetImage("UI_Main_Start_Clicked");
	UI_Main_Credit = ResourceMgr->GetImage("UI_Main_Credit");
	UI_Main_Credit_Click = ResourceMgr->GetImage("UI_Main_Credit_Click");
	UI_Main_Credit_Clicked = ResourceMgr->GetImage("UI_Main_Credit_Clicked");
	UI_Main_Exit = ResourceMgr->GetImage("UI_Main_Exit");
	UI_Main_Exit_Click = ResourceMgr->GetImage("UI_Main_Exit_Click");


	Start = VECTOR2(540, 380);
	Credit = VECTOR2(540, 450);
	Exit = VECTOR2(540, 520);

	isPressed_start = false;
	isPressed_credit = false;
	isPressed_exit = false;


	return S_OK;
}

void ScMainMenu::Update()
{
	//마우스 충돌범위 받기	
	POINT mouse = KeyMgr->GetMousePos();

	//Start
	if (mouse.x < Start.x + UI_Main_Start->width && mouse.x > Start.x
		&& mouse.y < Start.y + UI_Main_Start->height && mouse.y > Start.y)
	{
		if (KeyMgr->MouseUp())
			isPressed_start = true;
		else if (KeyMgr->MousePressed())
			isCrashed_start = 1;
		else
			isCrashed_start = 0;
	}
	else
		isCrashed_start = 0;

	//Credit
	if (mouse.x < Credit.x + UI_Main_Credit->width && mouse.x > Credit.x
		&& mouse.y < Credit.y + UI_Main_Credit->height && mouse.y > Credit.y)
	{
		if (KeyMgr->MouseUp())
			isPressed_credit = true;
		else if (KeyMgr->MousePressed())
			isCrashed_crdit = 1;
		else
			isCrashed_crdit = 0;
	}
	else
		isCrashed_crdit = 0;
	//Exit
	if (mouse.x < Exit.x + UI_Main_Exit->width && mouse.x > Exit.x
		&& mouse.y < Exit.y + UI_Main_Exit->height && mouse.y > Exit.y)
	{
		if (KeyMgr->MouseUp())
			isPressed_exit = true;
		else if (KeyMgr->MousePressed())
			isCrashed_exit = 1;
		else
			isCrashed_exit = 0;
	}
	else
		isCrashed_exit = 0;
	//scene 바꾸기
	if (isPressed_start == true)	SceneMgr->ChangeScene("InGame");
	if (isPressed_credit == true)	SceneMgr->ChangeScene("Credit");
	//종료하기
	if (isPressed_exit == true)		SendMessage(g_hWnd, WM_CLOSE, 0, 0);
}

void ScMainMenu::Render()
{
	//이미지 출력하기
	Renderer->SetWorld(VECTOR2(0, 0));
	if (m_sceneBackGroundImage) Renderer->Draw(m_sceneBackGroundImage);

	//start
	Renderer->SetWorld(Start);
	if (UI_Main_Start)
	{
		if (isCrashed_start != 1)
			Renderer->Draw(UI_Main_Start);
		else
			Renderer->Draw(UI_Main_Start_Click);
	}
	//credit
	Renderer->SetWorld(Credit);
	if (UI_Main_Credit)
	{
		if (isCrashed_crdit != 1)
			Renderer->Draw(UI_Main_Credit);
		else
			Renderer->Draw(UI_Main_Credit_Click);
	}
	//exit
	Renderer->SetWorld(Exit);
	if (UI_Main_Exit)
	{
		if (isCrashed_exit != 1)
			Renderer->Draw(UI_Main_Exit);
		else
			Renderer->Draw(UI_Main_Exit_Click);
	}

}

void ScMainMenu::Release()
{
}
