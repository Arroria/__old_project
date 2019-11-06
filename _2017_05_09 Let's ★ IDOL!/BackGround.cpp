#include "stdafx.h"
#include "BackGround.h"


CBackGround::CBackGround()
{
}
CBackGround::~CBackGround()
{
}


void CBackGround::Init()
{
	m_imageKey = "BackGround1";
}

void CBackGround::Update()
{
	if (GetAsyncKeyState('Q'))
		m_imageKey = "BackGround1";
	if (GetAsyncKeyState('W'))
		m_imageKey = "BackGround2";
	if (GetAsyncKeyState('E'))
		m_imageKey = "BackGround3";
	if (GetAsyncKeyState('R'))
		m_imageKey = "BackGround4";
	if (GetAsyncKeyState('T'))
		m_imageKey = "BackGround5";
	if (GetAsyncKeyState('Y'))
		m_imageKey = "BackGround6";
}

void CBackGround::Render()
{
	ResourceMgr->Draw(m_imageKey, 0, 0);
}

void CBackGround::Release()
{
}
