// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

// C 런타임 헤더 파일입니다.
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"WindowsCodecs.lib")
#include <wincodec.h>
#include <wincodecsdk.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <cassert>
#include <stdlib.h>
#include <iostream>
#include <tchar.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <conio.h>

using namespace std;

//========================================
//  ## 디파인문 선언 ##
//========================================

#define WINNAME (LPTSTR)(TEXT("Project_rpg"))

#define WINSTARTX 100
#define WINSTARTY 0
#define WINSIZEX 800
#define WINSIZEY 940

#define WINSTYLE WS_CAPTION | WS_SYSMENU

//========================================
// ## 매크로 함수 ##
//========================================

#define SAFE_DELETE(p)			{if(p) { delete(p); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p)		{if(p) { delete[](p); (p) = NULL;}}
#define SAFE_RELEASE(p)			{if(p) {(p)->Release(); (p) = NULL;}}

#if _DEBUG
#define DEBUG_LOG(log) cout << log << endl;
#else
#define DEBUG_LOG(log)
#endif //디버그용 함수


//========================================
// ## 전역 변수 선언 ##
//========================================

extern POINT _ptMouse;
extern HINSTANCE _hInstance;
extern HWND _hWnd;