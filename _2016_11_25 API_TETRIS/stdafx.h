// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

// C ��Ÿ�� ��� �����Դϴ�.
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
//  ## �����ι� ���� ##
//========================================

#define WINNAME (LPTSTR)(TEXT("Project_rpg"))

#define WINSTARTX 100
#define WINSTARTY 0
#define WINSIZEX 800
#define WINSIZEY 940

#define WINSTYLE WS_CAPTION | WS_SYSMENU

//========================================
// ## ��ũ�� �Լ� ##
//========================================

#define SAFE_DELETE(p)			{if(p) { delete(p); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p)		{if(p) { delete[](p); (p) = NULL;}}
#define SAFE_RELEASE(p)			{if(p) {(p)->Release(); (p) = NULL;}}

#if _DEBUG
#define DEBUG_LOG(log) cout << log << endl;
#else
#define DEBUG_LOG(log)
#endif //����׿� �Լ�


//========================================
// ## ���� ���� ���� ##
//========================================

extern POINT _ptMouse;
extern HINSTANCE _hInstance;
extern HWND _hWnd;