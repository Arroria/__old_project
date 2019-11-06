#include "stdafx.h"
#include "GameModeUI.h"

#include "BaseButton.h"

int		GameModeUI::m_gameSize[2] = { 4, 4 };
bool	GameModeUI::m_isHardestMode = false;
GameModeUI::GameModeUI(GameModeUI** thisPointer)
	: m_thisPointer(thisPointer)

	, m_btStart	(new BaseButton(D3DXVECTOR2(290, 600), D3DXVECTOR2(200, 80)))
	, m_btClose	(new BaseButton(D3DXVECTOR2(510, 600), D3DXVECTOR2(200, 80)))
	
	, m_btModeLite		(new BaseButton(D3DXVECTOR2(210, 350), D3DXVECTOR2(160, 80)))
	, m_btModeHardest	(new BaseButton(D3DXVECTOR2(400, 350), D3DXVECTOR2(160, 80)))
	, m_btModeExtend	(new BaseButton(D3DXVECTOR2(590, 350), D3DXVECTOR2(160, 80)))
	, m_btModeExtendSizingX	(new BaseButton(D3DXVECTOR2(590, 450), D3DXVECTOR2(40, 40)))
	, m_btModeExtendSizingY	(new BaseButton(D3DXVECTOR2(590, 500), D3DXVECTOR2(40, 40)))

	, m_selete(0)

	, m_font(nullptr)
{
	D3DXCreateFontW(g_processManager->GetDevice(), -30, 0, FW_DONTCARE, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"MUSECA", &m_font);
}
GameModeUI::~GameModeUI()
{
	SAFE_DELETE(m_btStart);
	SAFE_DELETE(m_btClose);

	SAFE_DELETE(m_btModeLite);
	SAFE_DELETE(m_btModeExtend);
	SAFE_DELETE(m_btModeHardest);
	SAFE_DELETE(m_btModeExtendSizingX);
	SAFE_DELETE(m_btModeExtendSizingY);

	SAFE_RELEASE(m_font);
}


void GameModeUI::Update()
{
	m_btStart->Update();
	if (m_btStart->IsClickOff() || g_handInputCalculator->IsClicked())//GetSingleton(InputManager)->IsKeyDown(VK_SPACE))
	{
		GetSingleton(SceneManager)->ChangeScene(L"InGame");
		return;
	}

	m_btClose->Update();
	if (m_btClose->IsClickOff())
	{
		*m_thisPointer = nullptr;
		delete this;
		return;
	}

	
	//Kinect
	bool seleteChanged = false;
	if (g_handInputCalculator->IsSlidedAtLeft())	{ seleteChanged = true; m_selete--; if (m_selete < 0) m_selete = 2; }
	if (g_handInputCalculator->IsSlidedAtRight())	{ seleteChanged = true; m_selete++; if (m_selete > 2) m_selete = 0; }
	
	if (seleteChanged)
	{
		switch (m_selete)
		{
		case 0:	goto LiteModeSeleted;		break;
		case 1:	goto HardestModeSeleted;	break;
		case 2:	goto ExtendModeSeleted;		break;
		}
	}


	m_btModeLite->Update();
	if (m_btModeLite->IsClickOff())
	{
		LiteModeSeleted:
		
		m_selete = 0;
		m_gameSize[0] = m_gameSize[1] = 4;
		m_isHardestMode = false;
		m_btModeExtendSizingX->SetPosition(D3DXVECTOR2(150 + (m_gameSize[0] - 2) * 500 / 6, 450));
		m_btModeExtendSizingY->SetPosition(D3DXVECTOR2(150 + (m_gameSize[1] - 2) * 500 / 6, 500));
	}
	m_btModeExtend->Update();
	if (m_btModeExtend->IsClickOff())
	{
		ExtendModeSeleted:

		m_selete = 2;
		m_isHardestMode = false;
		m_btModeExtendSizingX->SetPosition(D3DXVECTOR2(150 + (m_gameSize[0] - 2) * 500 / 6, 450));
		m_btModeExtendSizingY->SetPosition(D3DXVECTOR2(150 + (m_gameSize[1] - 2) * 500 / 6, 500));
	}
	m_btModeHardest->Update();
	if (m_btModeHardest->IsClickOff())
	{
		HardestModeSeleted:

		m_selete = 1;
		m_gameSize[0] = m_gameSize[1] = 4;
		m_isHardestMode = true;
	}

	m_btModeExtendSizingX->Update();
	m_btModeExtendSizingY->Update();
	if (m_selete == 2)
	{
		if (m_btModeExtendSizingX->IsPressed())
		{
			float buttonPosition = GetSingleton(InputManager)->GetMousePos().x;
			if (buttonPosition < 400 - 250)	buttonPosition = 0;
			else if (buttonPosition > 400 + 250)	buttonPosition = 500;
			else									buttonPosition -= 150;

			m_gameSize[0] = (int)((buttonPosition + (500 / 12)) / (500 / 6)) + 2;
			m_btModeExtendSizingX->SetPosition(D3DXVECTOR2(150 + (m_gameSize[0] - 2) * 500 / 6, 450));
		}
		else if (m_btModeExtendSizingY->IsPressed())
		{
			float buttonPosition = GetSingleton(InputManager)->GetMousePos().x;
			if (buttonPosition < 400 - 250)	buttonPosition = 0;
			else if (buttonPosition > 400 + 250)	buttonPosition = 500;
			else									buttonPosition -= 150;

			m_gameSize[1] = (int)((buttonPosition + (500 / 12)) / (500 / 6)) + 2;
			m_btModeExtendSizingY->SetPosition(D3DXVECTOR2(150 + (m_gameSize[1] - 2) * 500 / 6, 500));
		}
	}
}

void GameModeUI::Render()
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
		D3DXMatrixTranslation(&t, 400, 480, 0);

		#define BackgroundWidth  300
		#define BackgroundHeight 210

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

	//Button
	{
		//Start
		{
			D3DXVECTOR2 renderPosition(290, 600);


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
			m_font->DrawTextW(nullptr, L"START", -1, &(fontDrawPosition + 1), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"START", -1, &(fontDrawPosition - 1), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"START", -1, &(fontDrawPosition + POINT_ex(1, -1)), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"START", -1, &(fontDrawPosition - POINT_ex(-1, 1)), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"START", -1, &fontDrawPosition, DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(1, 1, 1, 1));
		}

		//Close
		{
			D3DXVECTOR2 renderPosition(510, 600);


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

		//Lite
		{
			D3DXVECTOR2 renderPosition(210, 350);


			for (auto& iter : plane)
				iter.color = D3DXCOLOR(0, 0, 0, 1);
			D3DXMATRIX s, t;
			D3DXMatrixTranslation(&t, renderPosition.x, renderPosition.y, 0);

			D3DXMatrixScaling(&s, 80, 40, 1);
			g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
			g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));

			for (auto& iter : plane)
			{
				m_selete == 0 ?
					iter.color = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1):
					iter.color = D3DXCOLOR(1, 1, 1, 1);
			}
			const float OutlineSize(5);
			D3DXMatrixScaling(&s, 80 - OutlineSize, 40 - OutlineSize, 1);
			g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
			g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));

			RECT_ex fontDrawPosition;	fontDrawPosition += POINT_ex(renderPosition.x, renderPosition.y);
			m_font->DrawTextW(nullptr, L"Lite", -1, &(fontDrawPosition + 1), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"Lite", -1, &(fontDrawPosition - 1), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"Lite", -1, &(fontDrawPosition + POINT_ex(1, -1)), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"Lite", -1, &(fontDrawPosition - POINT_ex(-1, 1)), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"Lite", -1, &fontDrawPosition, DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(1, 1, 1, 1));
		}

		//Hardest
		{
			D3DXVECTOR2 renderPosition(400, 350);


			for (auto& iter : plane)
				iter.color = D3DXCOLOR(0, 0, 0, 1);
			D3DXMATRIX s, t;
			D3DXMatrixTranslation(&t, renderPosition.x, renderPosition.y, 0);

			D3DXMatrixScaling(&s, 80, 40, 1);
			g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
			g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));

			for (auto& iter : plane)
			{
				m_selete == 1 ?
					iter.color = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1) :
					iter.color = D3DXCOLOR(1, 1, 1, 1);
			}
			const float OutlineSize(5);
			D3DXMatrixScaling(&s, 80 - OutlineSize, 40 - OutlineSize, 1);
			g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
			g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));

			RECT_ex fontDrawPosition;	fontDrawPosition += POINT_ex(renderPosition.x, renderPosition.y);
			m_font->DrawTextW(nullptr, L"Hardest", -1, &(fontDrawPosition + 1), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"Hardest", -1, &(fontDrawPosition - 1), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"Hardest", -1, &(fontDrawPosition + POINT_ex(1, -1)), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"Hardest", -1, &(fontDrawPosition - POINT_ex(-1, 1)), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"Hardest", -1, &fontDrawPosition, DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(1, 1, 1, 1));
		}

		//Extend
		{
			D3DXVECTOR2 renderPosition(590, 350);


			for (auto& iter : plane)
				iter.color = D3DXCOLOR(0, 0, 0, 1);
			D3DXMATRIX s, t;
			D3DXMatrixTranslation(&t, renderPosition.x, renderPosition.y, 0);

			D3DXMatrixScaling(&s, 80, 40, 1);
			g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
			g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));

			for (auto& iter : plane)
			{
				m_selete == 2 ?
					iter.color = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1) :
					iter.color = D3DXCOLOR(1, 1, 1, 1);
			}
			const float OutlineSize(5);
			D3DXMatrixScaling(&s, 80 - OutlineSize, 40 - OutlineSize, 1);
			g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
			g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));

			RECT_ex fontDrawPosition;	fontDrawPosition += POINT_ex(renderPosition.x, renderPosition.y);
			m_font->DrawTextW(nullptr, L"Extend", -1, &(fontDrawPosition + 1), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"Extend", -1, &(fontDrawPosition - 1), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"Extend", -1, &(fontDrawPosition + POINT_ex(1, -1)), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"Extend", -1, &(fontDrawPosition - POINT_ex(-1, 1)), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
			m_font->DrawTextW(nullptr, L"Extend", -1, &fontDrawPosition, DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(1, 1, 1, 1));
		}

		//Extend
		if (m_selete == 2)
		{
			//X
			{
				//Bar
				for (auto& iter : plane)
					iter.color = D3DXCOLOR(0, 0, 0, 1);
				D3DXMATRIX s, t;
				D3DXMatrixTranslation(&t, 400, 450, 0);

				D3DXMatrixScaling(&s, 250, 2.5f, 1);
				g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
				g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));

				//Button
				D3DXVECTOR2 renderPosition((400 - 250) + (m_gameSize[0] - 2) * 500 / 6, 450);
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

				RECT_ex fontDrawPosition;	fontDrawPosition += POINT_ex(renderPosition.x, renderPosition.y);
				WCHAR tempBuffer[1000] = { NULL };
				_itow(m_gameSize[0], tempBuffer, 10);
				m_font->DrawTextW(nullptr, tempBuffer, -1, &(fontDrawPosition + 1), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
				m_font->DrawTextW(nullptr, tempBuffer, -1, &(fontDrawPosition - 1), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
				m_font->DrawTextW(nullptr, tempBuffer, -1, &(fontDrawPosition + POINT_ex(1, -1)), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
				m_font->DrawTextW(nullptr, tempBuffer, -1, &(fontDrawPosition - POINT_ex(-1, 1)), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
				m_font->DrawTextW(nullptr, tempBuffer, -1, &fontDrawPosition, DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(1, 1, 1, 1));
			}
			//Y
			{
				//Bar
				for (auto& iter : plane)
					iter.color = D3DXCOLOR(0, 0, 0, 1);
				D3DXMATRIX s, t;
				D3DXMatrixTranslation(&t, 400, 500, 0);

				D3DXMatrixScaling(&s, 250, 2.5f, 1);
				g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
				g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &plane[0], sizeof(_SimplePlaneVertex));

				//Button
				D3DXVECTOR2 renderPosition((400 - 250) + (m_gameSize[1] - 2) * 500 / 6, 500);
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

				RECT_ex fontDrawPosition;	fontDrawPosition += POINT_ex(renderPosition.x, renderPosition.y);
				WCHAR tempBuffer[1000] = { NULL };
				_itow(m_gameSize[1], tempBuffer, 10);
				m_font->DrawTextW(nullptr, tempBuffer, -1, &(fontDrawPosition + 1), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
				m_font->DrawTextW(nullptr, tempBuffer, -1, &(fontDrawPosition - 1), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
				m_font->DrawTextW(nullptr, tempBuffer, -1, &(fontDrawPosition + POINT_ex(1, -1)), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
				m_font->DrawTextW(nullptr, tempBuffer, -1, &(fontDrawPosition - POINT_ex(-1, 1)), DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(0, 0, 0, 1));
				m_font->DrawTextW(nullptr, tempBuffer, -1, &fontDrawPosition, DT_NOCLIP | DT_VCENTER | DT_CENTER, D3DXCOLOR(1, 1, 1, 1));
			}
		}
	}
}

