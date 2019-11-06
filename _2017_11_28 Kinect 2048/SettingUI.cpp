#include "stdafx.h"
#include "SettingUI.h"

#include "BaseButton.h"

#define CloseButtonHeight			600
#define BoxSpeedButtonHeight		400
#define ScoreAnimationButtonHeight	500

float  SettingUI::m_boxMovementSpeed = 0.5f;
float  SettingUI::m_scoreAnimationSpeed = 1.5f;
SettingUI::SettingUI(SettingUI ** thisPointer)
	: m_thisPointer(thisPointer)

	, m_font(nullptr)
	 
	, m_close			(new BaseButton(D3DXVECTOR2(400, CloseButtonHeight), D3DXVECTOR2(200, 80)))
	, m_boxSpeedButton	(new BaseButton(D3DXVECTOR2(150 + m_boxMovementSpeed * 500, BoxSpeedButtonHeight), D3DXVECTOR2(40, 40)))
	, m_scoreSpeedButton(new BaseButton(D3DXVECTOR2(150 + m_scoreAnimationSpeed * 500 / 3, ScoreAnimationButtonHeight), D3DXVECTOR2(40, 40)))
{
	D3DXCreateFontW(g_processManager->GetDevice(), -30, 0, FW_DONTCARE, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"MUSECA", &m_font);
}
SettingUI::~SettingUI()
{
	SAFE_DELETE(m_close);
	SAFE_DELETE(m_boxSpeedButton);
	SAFE_DELETE(m_scoreSpeedButton);

	SAFE_RELEASE(m_font);
}


void SettingUI::Update()
{
	m_close->Update();
	if (m_close->IsClickOff() || g_handInputCalculator->IsClicked())
	{
		*m_thisPointer = nullptr;
		delete this;
		return;
	}

	m_boxSpeedButton->Update();
	if (m_boxSpeedButton->IsPressed())
	{
		float buttonPosition = GetSingleton(InputManager)->GetMousePos().x;
			 if (buttonPosition < 400 - 250)	buttonPosition = 0;
		else if (buttonPosition > 400 + 250)	buttonPosition = 500;
		else									buttonPosition -= 150;

		m_boxMovementSpeed = (int)((buttonPosition + (50 * 0.5f)) / 50) * 0.1f;
		m_boxSpeedButton->SetPosition(D3DXVECTOR2(150 + m_boxMovementSpeed * 500, BoxSpeedButtonHeight));
	}

	m_scoreSpeedButton->Update();
	if (m_scoreSpeedButton->IsPressed())
	{
		float buttonPosition = GetSingleton(InputManager)->GetMousePos().x;
			 if (buttonPosition < 400 - 250)	buttonPosition = 0;
		else if (buttonPosition > 400 + 250)	buttonPosition = 500;
		else									buttonPosition -= 150;

		m_scoreAnimationSpeed = (int)((buttonPosition + (500 / 12)) / (500 / 6)) * 0.5f;
		m_scoreSpeedButton->SetPosition(D3DXVECTOR2(150 + m_scoreAnimationSpeed * 500 / 3, ScoreAnimationButtonHeight));
	}
}

void SettingUI::Render()
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
	//Draw Background
	{
		D3DXMATRIX s, t;
		D3DXMatrixTranslation(&t, 400, 500, 0);

		#define BackgroundWidth  300
		#define BackgroundHeight 200

		for (auto& iter : plane)
			iter.color = D3DXCOLOR(0, 0, 0, 1);
		D3DXMatrixScaling(&s, BackgroundWidth, BackgroundHeight, 1);
		g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
		g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));

		for (auto& iter : plane)
			iter.color = D3DXCOLOR(1, 1, 1, 1);
		D3DXMatrixScaling(&s, BackgroundWidth - 1, BackgroundHeight - 1, 1);
		g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
		g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));

		for (auto& iter : plane)
			iter.color = D3DXCOLOR(0, 0, 0, 1);
		D3DXMatrixScaling(&s, BackgroundWidth - 5, BackgroundHeight - 5, 1);
		g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
		g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));

		for (auto& iter : plane)
			iter.color = D3DXCOLOR(0.8f, 1 ,0.8f, 1);
		D3DXMatrixScaling(&s, BackgroundWidth - 6, BackgroundHeight - 6, 0);
		g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
		g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));
	}

	//Draw Button
	{
		//Box movement speed
		{
			//Bar
			for (auto& iter : plane)
				iter.color = D3DXCOLOR(0, 0, 0, 1);
			D3DXMATRIX s, t;
			D3DXMatrixTranslation(&t, 400, BoxSpeedButtonHeight, 0);

			D3DXMatrixScaling(&s, 250, 2.5f, 1);
			g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
			g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));

			//Button
			D3DXVECTOR2 renderPosition((400 - 250) + m_boxMovementSpeed * 500, BoxSpeedButtonHeight);
			D3DXMatrixTranslation(&t, renderPosition.x, renderPosition.y, 0);

			for (auto& iter : plane)
				iter.color = D3DXCOLOR(0, 0, 0, 1);
			D3DXMatrixScaling(&s, 20, 20, 1);
			g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
			g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));
			
			for (auto& iter : plane)
				iter.color = D3DXCOLOR(1, 1, 1, 1);
			const float OutlineSize(5);
			D3DXMatrixScaling(&s, 20 - OutlineSize , 20 - OutlineSize, 1);
			g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
			g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));
			
			RECT_ex fontDrawPosition;	fontDrawPosition += POINT_ex(400, renderPosition.y - 50);
			m_font->DrawTextW(nullptr, L"Box Movement Speed", -1, &(fontDrawPosition + 1), DT_NOCLIP | DT_TOP | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"Box Movement Speed", -1, &(fontDrawPosition - 1), DT_NOCLIP | DT_TOP | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"Box Movement Speed", -1, &(fontDrawPosition + POINT_ex(1, -1)), DT_NOCLIP | DT_TOP | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"Box Movement Speed", -1, &(fontDrawPosition - POINT_ex(-1, 1)), DT_NOCLIP | DT_TOP | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"Box Movement Speed", -1, &fontDrawPosition, DT_NOCLIP | DT_TOP | DT_CENTER, D3DXCOLOR(1, 1, 1, 1));
		}

		//Score up speed
		{
			//Bar
			for (auto& iter : plane)
				iter.color = D3DXCOLOR(0, 0, 0, 1);
			D3DXMATRIX s, t;
			D3DXMatrixTranslation(&t, 400, ScoreAnimationButtonHeight, 0);

			D3DXMatrixScaling(&s, 250, 2.5f, 1);
			g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
			g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));

			//Button
			D3DXVECTOR2 renderPosition((400 - 250) + m_scoreAnimationSpeed * 500 / 3, ScoreAnimationButtonHeight);
			D3DXMatrixTranslation(&t, renderPosition.x, renderPosition.y, 0);

			for (auto& iter : plane)
				iter.color = D3DXCOLOR(0, 0, 0, 1);
			D3DXMatrixScaling(&s, 20, 20, 1);
			g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
			g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));

			for (auto& iter : plane)
				iter.color = D3DXCOLOR(1, 1, 1, 1);
			const float OutlineSize(5);
			D3DXMatrixScaling(&s, 20 - OutlineSize, 20 - OutlineSize, 1);
			g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
			g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));

			RECT_ex fontDrawPosition;	fontDrawPosition += POINT_ex(400, renderPosition.y - 50);
			m_font->DrawTextW(nullptr, L"Score Animation Speed", -1, &(fontDrawPosition + 1), DT_NOCLIP | DT_TOP | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"Score Animation Speed", -1, &(fontDrawPosition - 1), DT_NOCLIP | DT_TOP | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"Score Animation Speed", -1, &(fontDrawPosition + POINT_ex(1, -1)), DT_NOCLIP | DT_TOP | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"Score Animation Speed", -1, &(fontDrawPosition - POINT_ex(-1, 1)), DT_NOCLIP | DT_TOP | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"Score Animation Speed", -1, &fontDrawPosition, DT_NOCLIP | DT_TOP | DT_CENTER, D3DXCOLOR(1, 1, 1, 1));
		}

		//UI Close
		{
			D3DXVECTOR2 renderPosition(400, CloseButtonHeight);


			for (auto& iter : plane)
				iter.color = D3DXCOLOR(0, 0, 0, 1);
			D3DXMATRIX s, t;
			D3DXMatrixTranslation(&t, renderPosition.x, renderPosition.y, 0);

			D3DXMatrixScaling(&s, 100, 40, 1);
			g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
			g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));

			for (auto& iter : plane)
				iter.color = D3DXCOLOR(1, 1, 1, 1);
			const float OutlineSize(5);
			D3DXMatrixScaling(&s, 100 - OutlineSize, 40 - OutlineSize, 1);
			g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
			g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));

			RECT_ex fontDrawPosition;	fontDrawPosition += POINT_ex(renderPosition.x, renderPosition.y);
			m_font->DrawTextW(nullptr, L"CLOSE", -1, &(fontDrawPosition + 1), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"CLOSE", -1, &(fontDrawPosition - 1), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"CLOSE", -1, &(fontDrawPosition + POINT_ex(1, -1)), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"CLOSE", -1, &(fontDrawPosition - POINT_ex(-1, 1)), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"CLOSE", -1, &fontDrawPosition, DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(1, 1, 1, 1));
		}
	}
}
