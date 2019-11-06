#include "stdafx.h"
#include "ScCredit.h"


ScCredit::ScCredit()
{
}
ScCredit::~ScCredit()
{
}



HRESULT ScCredit::Init()
{

	//���ι�� �޾ƿ���
	m_sceneBackGroundImage = ResourceMgr->GetImage("BG_Credit");
	return S_OK;
}

void ScCredit::Update()
{
	if (GetAsyncKeyState(VK_SPACE))
		SceneMgr->ChangeScene("MainMenu");
}

void ScCredit::Render()
{
	Renderer->SetWorld(VECTOR2(0, 0));
	if (m_sceneBackGroundImage) Renderer->Draw(m_sceneBackGroundImage);

	Renderer->DrawString(VECTOR2(100, 150), L"Graphic", 60, RGB(255, 200, 200));
	Renderer->DrawString(VECTOR2(450, 150), L"Programing", 60, RGB(200, 255, 255));
	Renderer->DrawString(VECTOR2(150, 300), L"ȫ����", 40, RGB(255, 200, 200));
	Renderer->DrawString(VECTOR2(150, 400), L"�ݸ��", 40, RGB(255, 200, 200));
	Renderer->DrawString(VECTOR2(540, 300), L"�ǽ¹�", 40, RGB(200, 255, 255));
	Renderer->DrawString(VECTOR2(540, 400), L"��ټ�", 40, RGB(200, 255, 255));
	Renderer->DrawString(VECTOR2(520, 550), L"PRESS \"SPACE\"", 30, RGB(200, 255, 200));
}

void ScCredit::Release()
{
}
