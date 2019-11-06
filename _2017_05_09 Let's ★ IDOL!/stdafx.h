// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <time.h>


extern size_t	g_winSizeX;
extern size_t	g_winSizeY;
extern int		g_winPosX;
extern int		g_winPosY;

//struct
typedef	struct _VECTOR2
{
	float x;
	float y;
	_VECTOR2() : x(0) ,y(0) {}
	_VECTOR2(const float& _x, const float& _y) : x(_x), y(_y) {}
}VECTOR2;


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#pragma comment (lib, "Msimg32.lib")	//TransparentBlt

#define SHUTDOWN_KEY VK_ESCAPE


#define SAFE_DELETE(p)			{ if(p) { delete p; p = nullptr; } }
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] p; p = nullptr; } }
#define SAFE_RELEASE(p)			{ if(p) { p->Release(); p = nullptr; } }
#define PURE		=0



#define WIN_CLASSNAME	L"_Let's ★ IDOL!"
#define WIN_TITLE		L"Let's ★ IDOL!"
#define WIN_STYLE		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX
#define WIN_SIZEX		800				   
#define WIN_SIZEY		600
#define WIN_POSX		(0.5f * (GetSystemMetrics(SM_CXSCREEN) - WIN_SIZEX))
#define WIN_POSY		(0.5f * (GetSystemMetrics(SM_CYSCREEN) - WIN_SIZEY))


#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "GameObject.h"