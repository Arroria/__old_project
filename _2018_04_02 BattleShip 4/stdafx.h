#pragma once

#define WND_NAME L""
#define WND_ISWND true
#define WND_SIZEX 1600
#define WND_SIZEY 900

#include <array>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <iostream>
#include <windowsx.h>
#include <functional>

#include "./DXUT/Optional/SDKsound.h"
#pragma comment (lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/dsound.lib")

#define DEVICE	(DXUTGetD3D9Device())
using std::cout;
using std::wcout;
using std::endl;


#include "Plane.h"
#include "Camera.h"
#include "WhiteMtrl.h"
#include "SkySphere.h"
#include "BillBoard.h"
#include "ZBufferSetter.h"
#include "Sprite.h"

#include "InputManager.h"
#include "SceneManager.h"

#include "TextureManager.h"
#include "ASEManager.h"


