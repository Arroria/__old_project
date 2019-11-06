#include "stdafx.h"
#include "MainTitle.h"

#include "GameModeUI.h"
#include "SettingUI.h"

#define g_device (g_processManager->GetDevice())

MainTitle::MainTitle()
	: m_titleFont(nullptr)
	, m_menuFont(nullptr)
	, m_startPosition	(-75, 0, 75, 50)
	, m_optionPosition	(-90, 0, 90, 50)
	, m_exitPosition	(-55, 0, 55, 50)

	, m_seletedMenu(0)

	, m_gameModeUI(nullptr)
	, m_settingUI(nullptr)
{
	m_startPosition		+= POINT_ex(g_processManager->GetWndInfo()->xSize * 0.5f, 450);
	m_optionPosition	+= POINT_ex(g_processManager->GetWndInfo()->xSize * 0.5f, 550);
	m_exitPosition		+= POINT_ex(g_processManager->GetWndInfo()->xSize * 0.5f, 650);
}
MainTitle::~MainTitle()
{
}


void MainTitle::Activated()
{
	D3DXCreateFontW(g_device, -160, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"MUSECA", &m_titleFont);
	D3DXCreateFontW(g_device, -50, 0, FW_DONTCARE, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"MUSECA", &m_menuFont);
}

void MainTitle::Update()
{
	if (m_gameModeUI)
	{
		m_gameModeUI->Update();
	}
	else if (m_settingUI)
	{
		m_settingUI->Update();
	}
	else
	{
		auto MouseCrashedWithMenu = [](const POINT_ex& mousePos, const RECT_ex& menuRect)->bool
		{
			return
				menuRect.left < mousePos.x &&	mousePos.x < menuRect.right &&
				menuRect.top < mousePos.y && mousePos.y < menuRect.bottom;
		};

		//Change Selete
		if (g_handInputCalculator->IsClicked())
		///	if (GetSingleton(InputManager)->IsKeyDown(VK_SPACE) || GetSingleton(InputManager)->IsKeyDown(VK_RBUTTON))
		{
			switch (m_seletedMenu)
			{
			case 0:
				m_gameModeUI = new GameModeUI(&m_gameModeUI);
				break;
				//GetSingleton(SceneManager)->ChangeScene(L"InGame");
			case 1:
				m_settingUI = new SettingUI(&m_settingUI);
				break;
			case 2:
				PostQuitMessage(0);
				break;
			}
		}
		else
		{
			if (g_handInputCalculator->IsSlidedAtUp())		{ m_seletedMenu--; if (m_seletedMenu < 0) m_seletedMenu = 2; }
			if (g_handInputCalculator->IsSlidedAtDown())	{ m_seletedMenu++; if (m_seletedMenu > 2) m_seletedMenu = 0; }
			
			///		 if (GetSingleton(InputManager)->IsKeyDown(VK_LBUTTON) && MouseCrashedWithMenu(GetSingleton(InputManager)->GetMousePos(), m_startPosition))		m_seletedMenu = 0;
			///	else if (GetSingleton(InputManager)->IsKeyDown(VK_LBUTTON) && MouseCrashedWithMenu(GetSingleton(InputManager)->GetMousePos(), m_optionPosition))	m_seletedMenu = 1;
			///	else if (GetSingleton(InputManager)->IsKeyDown(VK_LBUTTON) && MouseCrashedWithMenu(GetSingleton(InputManager)->GetMousePos(), m_exitPosition))		m_seletedMenu = 2;
			
			///	else if (GetSingleton(InputManager)->IsKeyDown(VK_UP) || GetSingleton(InputManager)->IsKeyDown('W')) { m_seletedMenu--; if (m_seletedMenu < 0) m_seletedMenu = 2; }
			///	else if (GetSingleton(InputManager)->IsKeyDown(VK_DOWN) || GetSingleton(InputManager)->IsKeyDown('S')) { m_seletedMenu++; if (m_seletedMenu > 2) m_seletedMenu = 0; }
		}
	}
}

void MainTitle::Render()
{
	//Render Title
	{
		RECT_ex renderPosition(0, 150, g_processManager->GetWndInfo()->xSize, 0);
		m_titleFont->DrawTextW(nullptr, L"2048", -1, &(renderPosition - 1),					DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(0, 0, 0, 1));
		m_titleFont->DrawTextW(nullptr, L"2048", -1, &(renderPosition + 1),					DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(0, 0, 0, 1));
		m_titleFont->DrawTextW(nullptr, L"2048", -1, &(renderPosition + POINT_ex(-1, 1)),	DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(0, 0, 0, 1));
		m_titleFont->DrawTextW(nullptr, L"2048", -1, &(renderPosition + POINT_ex(1, -1)),	DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(0, 0, 0, 1));
		m_titleFont->DrawTextW(nullptr, L"2048", -1, & renderPosition, DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(1, 1, 1, 1));
	}

	//Render Menu
	{
		//Render Start Button
		{
			//Render Plane

			//Render Text
			m_menuFont->DrawTextW(nullptr, L"Start", -1, &(m_startPosition - 1), DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"Start", -1, &(m_startPosition + 1), DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"Start", -1, &(m_startPosition + POINT_ex(-1, 1)), DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"Start", -1, &(m_startPosition + POINT_ex(1, -1)), DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"Start", -1, & m_startPosition, DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(1, 1, (m_seletedMenu == 0 ? 0 : 1), 1));
		}

		//Render Option Button
		{
			//Render Plane

			//Render Text
			m_menuFont->DrawTextW(nullptr, L"Option", -1, &(m_optionPosition - 1), DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"Option", -1, &(m_optionPosition + 1), DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"Option", -1, &(m_optionPosition + POINT_ex(-1, 1)), DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"Option", -1, &(m_optionPosition + POINT_ex(1, -1)), DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"Option", -1, & m_optionPosition, DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(1, 1, (m_seletedMenu == 1 ? 0 : 1), 1));
		}

		//Render Exit Button
		{
			//Render Plane

			//Render Text
			m_menuFont->DrawTextW(nullptr, L"Exit", -1, &(m_exitPosition - 1), DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"Exit", -1, &(m_exitPosition + 1), DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"Exit", -1, &(m_exitPosition + POINT_ex(-1, 1)), DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"Exit", -1, &(m_exitPosition + POINT_ex(1, -1)), DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"Exit", -1, & m_exitPosition, DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(1, 1, (m_seletedMenu == 2 ? 0 : 1), 1));
		}
		
		//RenderSeleted
		{
			RECT_ex renderPosition;
			switch (m_seletedMenu)
			{
			case 0:	renderPosition = m_startPosition;	break;
			case 1:	renderPosition = m_optionPosition;	break;
			case 2:	renderPosition = m_exitPosition;	break;
			}
			m_menuFont->DrawTextW(nullptr, L"-                -", -1, &(renderPosition - 1), DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"-                -", -1, &(renderPosition + 1), DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"-                -", -1, &(renderPosition + POINT_ex(-1, 1)), DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"-                -", -1, &(renderPosition + POINT_ex(1, -1)), DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"-                -", -1, &renderPosition, DT_NOCLIP | DT_CENTER | DT_TOP, D3DXCOLOR(1, 1, 0, 1));
		}
	}

	if (m_gameModeUI)
		m_gameModeUI->Render();
	else if (m_settingUI)
		m_settingUI->Render();
}

void MainTitle::Disabled()
{
	SAFE_DELETE(m_gameModeUI);
	SAFE_DELETE(m_settingUI);

	SAFE_RELEASE(m_titleFont);
	SAFE_RELEASE(m_menuFont);
}

