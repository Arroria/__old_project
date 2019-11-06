#pragma once
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <vector>
#include <list>
#include <string>
#include <map>
#include <windowsx.h>
#include <iostream>
using std::cout;
using std::wcout;
using std::endl;


#include "C:/Program Files (x86)//Microsoft DirectX SDK (June 2010)/Include/d3d9.h"
#include "C:/Program Files (x86)//Microsoft DirectX SDK (June 2010)/Include/d3dx9.h"
#pragma comment (lib, "C:/Program Files (x86)//Microsoft DirectX SDK (June 2010)/lib/x86/d3d9.lib")
#pragma comment (lib, "C:/Program Files (x86)//Microsoft DirectX SDK (June 2010)/lib/x86/d3dx9.lib")

#define SAFE_DELETE(p)			{ if (p) { delete p; p = nullptr; }		}
#define SAFE_DELETE_ARRAY(p)	{ if (p) { delete[] p; p = nullptr; }	}
#define SAFE_RELEASE(p)			{ if (p) { p->Release(); p = nullptr; }	}

#include "Singleton.h"

#include "ProcessManager.h"
extern ProcessManager* g_processManager;

#include "TextureManager.h"
extern TextureContainer* g_textureContainer;

#include "SceneManager.h"
#include "InputManager.h"

#include "ASE.h"
extern ASELoader* g_ase;

