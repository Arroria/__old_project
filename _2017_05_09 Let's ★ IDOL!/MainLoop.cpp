#include "stdafx.h"
#include "MainLoop.h"

CMainLoop::CMainLoop()
{
}
CMainLoop::~CMainLoop()
{
}



//������ ������ ����
int CreateDevice(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	srand((unsigned int)time(NULL));
	ResourceMgr->Init(hWnd);
	SceneMgr->Init();

	return 1;
}

//������ ����� ����
int ResetDevice(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	ResourceMgr->Release();
	SceneMgr->Release();

	ResourceMgr_Release;
	SceneMgr_Release;
	KeyMgr_Release;

	PostQuitMessage(0);

	return 1;
}



//���� �ʱ�ȭ
void CMainLoop::Init()
{
}

//���� ������Ʈ
void CMainLoop::Update()
{
	KeyMgr->Update();
	SceneMgr->Update();
}

//���� ��� ����
void CMainLoop::Render()
{
	ResourceMgr->Begin();
	SceneMgr->Render();
	ResourceMgr->End();
}

//���� ����
void CMainLoop::Release()
{

}
