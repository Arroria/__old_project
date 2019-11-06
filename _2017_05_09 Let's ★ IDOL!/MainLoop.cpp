#include "stdafx.h"
#include "MainLoop.h"

CMainLoop::CMainLoop()
{
}
CMainLoop::~CMainLoop()
{
}



//윈도우 생성시 실행
int CreateDevice(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	srand((unsigned int)time(NULL));
	ResourceMgr->Init(hWnd);
	SceneMgr->Init();

	return 1;
}

//윈도우 종료시 실행
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



//게임 초기화
void CMainLoop::Init()
{
}

//게임 업데이트
void CMainLoop::Update()
{
	KeyMgr->Update();
	SceneMgr->Update();
}

//게임 요소 렌더
void CMainLoop::Render()
{
	ResourceMgr->Begin();
	SceneMgr->Render();
	ResourceMgr->End();
}

//게임 해제
void CMainLoop::Release()
{

}
