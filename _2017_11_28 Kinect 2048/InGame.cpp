#include "stdafx.h"
#include "InGame.h"

#include "BaseButton.h"

#include "GameWorld.h"
#include "GameModeUI.h"
#include "SettingUI.h"
#define ScoreAnimationTime SettingUI::GetScoreAnimationSpeed()

InGame::InGame()
	: m_gameWorld(nullptr)
	, m_settingUI(nullptr)

	, m_titleFont(nullptr)
	, m_scoreFont(nullptr)
	, m_menuFont(nullptr)

	, m_prevSocre(0)
	, m_nowScore(0)
	, m_objectiveScore(0)
	, m_elapsedTime(0)

	, m_settingUICallButton	(new BaseButton(D3DXVECTOR2(690, 110), D3DXVECTOR2(100, 40)))
	, m_goMain				(new BaseButton(D3DXVECTOR2(690, 50), D3DXVECTOR2(100, 40)))
{
	D3DXCreateFontW(g_processManager->GetDevice(), -100, 0, FW_DONTCARE, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"MUSECA", &m_titleFont);
	D3DXCreateFontW(g_processManager->GetDevice(), -50, 0, FW_DONTCARE, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"MUSECA", &m_scoreFont);
	D3DXCreateFontW(g_processManager->GetDevice(), -20, 0, FW_DONTCARE, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"MUSECA", &m_menuFont);
}
InGame::~InGame()
{
	SAFE_RELEASE(m_titleFont);
	SAFE_RELEASE(m_scoreFont);
	SAFE_RELEASE(m_menuFont);

	SAFE_DELETE(m_settingUICallButton);
	SAFE_DELETE(m_goMain);
}


void InGame::Activated()
{
	m_prevSocre = m_nowScore = m_objectiveScore = NULL;

	m_gameWorld = new GameWorld(POINT_ex(GameModeUI::GetGameSize()[0], GameModeUI::GetGameSize()[1]));
	if (GameModeUI::IsHardestMode())
		m_gameWorld->CreateBlock(-1);
}

void InGame::Update()
{
	if (m_settingUI)
		m_settingUI->Update();
	else
	{
		if (GetSingleton(InputManager)->IsKeyDown('R'))
		{
			this->Disabled();
			this->Activated();
			return;
		}

		m_gameWorld->Update();

		if (g_handInputCalculator->IsSlidedAtUp())		m_gameWorld->MoveBox(MoveDirection::Top);
		if (g_handInputCalculator->IsSlidedAtDown())	m_gameWorld->MoveBox(MoveDirection::Bottom);
		if (g_handInputCalculator->IsSlidedAtLeft())	m_gameWorld->MoveBox(MoveDirection::Left);
		if (g_handInputCalculator->IsSlidedAtRight())	m_gameWorld->MoveBox(MoveDirection::Right);
		//	 if (GetSingleton(InputManager)->IsKeyDown('A') || GetSingleton(InputManager)->IsKeyDown(VK_LEFT))	m_gameWorld->MoveBox(MoveDirection::Left);
		//else if (GetSingleton(InputManager)->IsKeyDown('W') || GetSingleton(InputManager)->IsKeyDown(VK_UP))	m_gameWorld->MoveBox(MoveDirection::Top);
		//else if (GetSingleton(InputManager)->IsKeyDown('D') || GetSingleton(InputManager)->IsKeyDown(VK_RIGHT))	m_gameWorld->MoveBox(MoveDirection::Right);
		//else if (GetSingleton(InputManager)->IsKeyDown('S') || GetSingleton(InputManager)->IsKeyDown(VK_DOWN))	m_gameWorld->MoveBox(MoveDirection::Bottom);


		//Score Carculate
		{
			if (m_objectiveScore != m_gameWorld->GetScore())
			{
				m_prevSocre = m_nowScore;
				m_objectiveScore = m_gameWorld->GetScore();
				m_elapsedTime = ScoreAnimationTime;
			}

			if (m_elapsedTime > 0)
			{
				if ((m_elapsedTime -= g_processManager->GetDeltaTime()) < 0)
					m_elapsedTime = 0;

				double scoreRatio(sqrt(1 - pow(m_elapsedTime / ScoreAnimationTime, 2)));
				m_nowScore = m_prevSocre + (m_objectiveScore - m_prevSocre) * scoreRatio;
			}
			else
				m_nowScore = m_objectiveScore;
		}

		m_settingUICallButton->Update();
		if (m_settingUICallButton->IsClickOff())
			m_settingUI = new SettingUI(&m_settingUI);

		m_goMain->Update();
		if (m_goMain->IsClickOff())
		{
			GetSingleton(SceneManager)->ChangeScene(L"Title");
			return;
		}
	}
}

void InGame::Render()
{
	//Draw Element
	{
		//Title
		{
			std::wstring scoreOutput(L"2048");
			RECT_ex tempRect(400, NULL, 400, 100);
			//Outline
			{
				RECT_ex outlineRect;
				m_titleFont->DrawTextW(NULL, scoreOutput.c_str(), -1, &(tempRect + 1), DT_NOCLIP | DT_CENTER | DT_BOTTOM, D3DXCOLOR(0, 0, 0, 1));
				m_titleFont->DrawTextW(NULL, scoreOutput.c_str(), -1, &(tempRect + 1), DT_NOCLIP | DT_CENTER | DT_BOTTOM, D3DXCOLOR(0, 0, 0, 1));
				outlineRect = tempRect;	tempRect.left--;	tempRect.top++;	tempRect.right--;	tempRect.bottom++;
				m_titleFont->DrawTextW(NULL, scoreOutput.c_str(), -1, &outlineRect, DT_NOCLIP | DT_CENTER | DT_BOTTOM, D3DXCOLOR(0, 0, 0, 1));
				outlineRect = tempRect;	tempRect.left++;	tempRect.top--;	tempRect.right++;	tempRect.bottom--;
				m_titleFont->DrawTextW(NULL, scoreOutput.c_str(), -1, &outlineRect, DT_NOCLIP | DT_CENTER | DT_BOTTOM, D3DXCOLOR(0, 0, 0, 1));
			}
			m_titleFont->DrawTextW(NULL, scoreOutput.c_str(), -1, &tempRect, DT_NOCLIP | DT_CENTER | DT_BOTTOM, D3DXCOLOR(1, 1, 1, 1));
		}

		//Score
		{
			WCHAR tempBuffer[1000] = { NULL };
			std::wstring scoreOutput(std::wstring(L"Score : ") + _i64tow(m_nowScore, tempBuffer, 10));
			RECT_ex tempRect(100, NULL , NULL, 200);
			//Outline
			{
				RECT_ex outlineRect;
				m_scoreFont->DrawTextW(NULL, scoreOutput.c_str(), -1, &(tempRect + 1), DT_NOCLIP | DT_LEFT | DT_BOTTOM, D3DXCOLOR(0, 0, 0, 1));
				m_scoreFont->DrawTextW(NULL, scoreOutput.c_str(), -1, &(tempRect + 1), DT_NOCLIP | DT_LEFT | DT_BOTTOM, D3DXCOLOR(0, 0, 0, 1));
				outlineRect = tempRect;	tempRect.left--;	tempRect.top++;	tempRect.right--;	tempRect.bottom++;
				m_scoreFont->DrawTextW(NULL, scoreOutput.c_str(), -1, &outlineRect, DT_NOCLIP | DT_LEFT | DT_BOTTOM, D3DXCOLOR(0, 0, 0, 1));
				outlineRect = tempRect;	tempRect.left++;	tempRect.top--;	tempRect.right++;	tempRect.bottom--;
				m_scoreFont->DrawTextW(NULL, scoreOutput.c_str(), -1, &outlineRect, DT_NOCLIP | DT_LEFT | DT_BOTTOM, D3DXCOLOR(0, 0, 0, 1));
			}
			m_scoreFont->DrawTextW(NULL, scoreOutput.c_str(), -1, &tempRect, DT_NOCLIP | DT_LEFT | DT_BOTTOM, D3DXCOLOR(1, 1, 1, 1));
		}

		//Setting Caller
		{
			//CreatePlan
			struct _SimplePlaneVertex
			{
				D3DXVECTOR3 pos;
				D3DCOLOR color;
				_SimplePlaneVertex(D3DXVECTOR3 pos, D3DCOLOR color) : pos(pos), color(color) {}
				enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
			};
			std::vector<_SimplePlaneVertex> plane;
			{
				plane.push_back(_SimplePlaneVertex(D3DXVECTOR3(-1, +1, 0), D3DXCOLOR(1, 1, 1, 1)));
				plane.push_back(_SimplePlaneVertex(D3DXVECTOR3(+1, -1, 0), D3DXCOLOR(1, 1, 1, 1)));
				plane.push_back(_SimplePlaneVertex(D3DXVECTOR3(+1, +1, 0), D3DXCOLOR(1, 1, 1, 1)));
				plane.push_back(_SimplePlaneVertex(D3DXVECTOR3(-1, +1, 0), D3DXCOLOR(1, 1, 1, 1)));
				plane.push_back(_SimplePlaneVertex(D3DXVECTOR3(-1, -1, 0), D3DXCOLOR(1, 1, 1, 1)));
				plane.push_back(_SimplePlaneVertex(D3DXVECTOR3(+1, -1, 0), D3DXCOLOR(1, 1, 1, 1)));
			}

			g_processManager->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
			g_processManager->GetDevice()->SetFVF(_SimplePlaneVertex::FVF);

			//Draw
			D3DXMATRIX s, t;
			D3DXMatrixTranslation(&t, 690, 110, 0);

			for (auto& iter : plane)
				iter.color = D3DXCOLOR(0, 0, 0, 1);
			D3DXMatrixScaling(&s, 50, 20, 1);
			g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
			g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));

			for (auto& iter : plane)
				iter.color = D3DXCOLOR(1, 1, 1, 1);
			D3DXMatrixScaling(&s, 48, 18, 1);
			g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
			g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));

			RECT_ex fontDrawPosition;	fontDrawPosition += POINT_ex(690, 110);
			m_menuFont->DrawTextW(nullptr, L"Setting", -1, &(fontDrawPosition + 1), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"Setting", -1, &(fontDrawPosition - 1), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"Setting", -1, &(fontDrawPosition + POINT_ex(1, -1)), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"Setting", -1, &(fontDrawPosition - POINT_ex(-1, 1)), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"Setting", -1, &fontDrawPosition, DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(1, 1, 1, 1));
		}

		//GoMain
		{
			//CreatePlan
			struct _SimplePlaneVertex
			{
				D3DXVECTOR3 pos;
				D3DCOLOR color;
				_SimplePlaneVertex(D3DXVECTOR3 pos, D3DCOLOR color) : pos(pos), color(color) {}
				enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
			};
			std::vector<_SimplePlaneVertex> plane;
			{
				plane.push_back(_SimplePlaneVertex(D3DXVECTOR3(-1, +1, 0), D3DXCOLOR(1, 1, 1, 1)));
				plane.push_back(_SimplePlaneVertex(D3DXVECTOR3(+1, -1, 0), D3DXCOLOR(1, 1, 1, 1)));
				plane.push_back(_SimplePlaneVertex(D3DXVECTOR3(+1, +1, 0), D3DXCOLOR(1, 1, 1, 1)));
				plane.push_back(_SimplePlaneVertex(D3DXVECTOR3(-1, +1, 0), D3DXCOLOR(1, 1, 1, 1)));
				plane.push_back(_SimplePlaneVertex(D3DXVECTOR3(-1, -1, 0), D3DXCOLOR(1, 1, 1, 1)));
				plane.push_back(_SimplePlaneVertex(D3DXVECTOR3(+1, -1, 0), D3DXCOLOR(1, 1, 1, 1)));
			}

			g_processManager->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
			g_processManager->GetDevice()->SetFVF(_SimplePlaneVertex::FVF);

			//Draw
			D3DXMATRIX s, t;
			D3DXMatrixTranslation(&t, 690, 50, 0);

			for (auto& iter : plane)
				iter.color = D3DXCOLOR(0, 0, 0, 1);
			D3DXMatrixScaling(&s, 50, 20, 1);
			g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
			g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));

			for (auto& iter : plane)
				iter.color = D3DXCOLOR(1, 1, 1, 1);
			D3DXMatrixScaling(&s, 48, 18, 1);
			g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
			g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));

			RECT_ex fontDrawPosition;	fontDrawPosition += POINT_ex(690, 50);
			m_menuFont->DrawTextW(nullptr, L"Title", -1, &(fontDrawPosition + 1), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"Title", -1, &(fontDrawPosition - 1), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"Title", -1, &(fontDrawPosition + POINT_ex(1, -1)), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"Title", -1, &(fontDrawPosition - POINT_ex(-1, 1)), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_menuFont->DrawTextW(nullptr, L"Title", -1, &fontDrawPosition, DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(1, 1, 1, 1));
		}
	}

	m_gameWorld->Render(D3DXVECTOR2(400, 600), D3DXVECTOR2(750, 750));
	if (m_settingUI)
		m_settingUI->Render();
}

void InGame::Disabled()
{
	SAFE_DELETE(m_gameWorld);
	SAFE_DELETE(m_settingUI);
}
