#pragma once
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.

#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 4996 )

#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <list>
#include <vector>
#include <string>
#include <map>
#include <windowsx.h>
#include <array>
#include <iostream>
using std::cout;
using std::wcout;
using std::endl;

#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3d9.h"
#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3dx9.h"
#pragma comment (lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/lib/x86/d3d9.lib")
#pragma comment (lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/lib/x86/d3dx9.lib")

#include "ProcessManager.h"
extern ProcessManager* g_processManager;
#define DEVICE (g_processManager->GetDevice())

#define SAFE_DELETE(p)			{ auto& _p = p; if (_p) { delete (_p); (_p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p)	{ auto& _p = p; if (_p) { delete[] (_p); (_p) = nullptr; } }
#define SAFE_RELEASE(p)			{ auto& _p = p; if (_p) { (_p)->Release(); (_p) = nullptr; } }
#define SAFE_RELEASE_DELETE(p)	{ auto& _p = p; if (_p) { (_p)->Release(); delete (_p); (_p) = nullptr; } }


#include "Expand.h"

#include "SceneManager.h"
#include "TextureLoader.h"
#include "PlaneRenderer.h"
#include "InputManager.h"
#include "Camera.h"
//#include "OBBCollider.h"
#include "Collision.h"

extern UINT g_score;
