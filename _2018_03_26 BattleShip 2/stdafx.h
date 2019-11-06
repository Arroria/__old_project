#pragma once

#define IS_WND		true
#define WND_SIZE_X	1600
#define WND_SIZE_Y	900
#define WND_NAME	L"BottleShip"

#include <list>
#include <map>
#include <vector>
#include <string>
#include <array>
#include <windowsx.h>
#include <iostream>
using std::cout;
using std::endl;

#include "./DXUT/Optional/SDKsound.h"
#pragma comment (lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/dsound.lib")

#define DEVICE (DXUTGetD3D9Device())

#include "Plane.h"
#include "Camera.h"
#include "SkySphere.h"
#include "Sprite.h"
#include "Fader.h"

#include "InputManager.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "ASEManager.h"
#include "SoundManager.h"

