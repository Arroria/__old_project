#pragma once
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.


#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <list>
#include <map>
#include <vector>
#include <string>
#include <array>
#include <windowsx.h>
#include <iostream>

#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3d9.h"
#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3dx9.h"
#pragma comment (lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/lib/x86/d3d9.lib")
#pragma comment (lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/lib/x86/d3dx9.lib")

#include "Expand.h"
#include "ProcessManager.h"
extern ProcessManager* g_processManager;
#define DEVICE (g_processManager->GetDevice())

#define SAFE_DELETE(p)			{ if (p) { delete (p); (p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p)	{ if (p) { delete[] (p); (p) = nullptr; } }
#define SAFE_RELEASE(p)			{ if (p) { (p)->Release(); (p) = nullptr; } }
#define SAFE_RELEASE_DELETE(p)	{ if (p) { (p)->Release(); delete (p); (p) = nullptr; } }

#include "InputManager.h"
#include "ASELoader.h"
#include "CameraManager.h"
#include "OBBCollision.h"
#include "SceneManager.h"
