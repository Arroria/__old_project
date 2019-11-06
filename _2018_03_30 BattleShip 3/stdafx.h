#pragma once

#define WND_ISWND true
#define WND_NAME L"Bottle Ship"
#define WND_SIZE_X 1600
#define WND_SIZE_Y 900

#define DEVICE (DXUTGetD3D9Device())

#include "SDKsound.h"
#pragma comment (lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/dsound.lib")

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <array>
#include <windowsx.h>
#include <functional>

using std::cout;
using std::wcout;
using std::endl;


#define RADIAN(degree) D3DXToRadian(degree)
#define DEGREE(radian) D3DXToDegree(radian)

#define ZBufferOn()		(DEVICE->SetRenderState(D3DRS_ZENABLE, true))
#define ZBufferOff()	(DEVICE->SetRenderState(D3DRS_ZENABLE, false))
#define ZBufWriteOn()	(DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true))
#define ZBufWriteOff()	(DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, false))

#include "Plane.h"
#include "Camera.h"
#include "WhiteMaterial.h"
#include "SkySphere.h"
#include "BillBoarder.h"

#include "TextureManager.h"
#include "ASEManager.h"

#include "InputManager.h"
#include "SceneManager.h"
