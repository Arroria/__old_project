#pragma once
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <iostream>
using std::cout;
using std::endl;

#include <string>
#include <list>
#include <vector>
#include <map>
#include <windowsx.h>

#include <Kinect.h>
#pragma comment (lib, "C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409/Lib/x86/Kinect20.lib")

#define SAFE_DELETE(p)			{ if (p) { delete p;	p = nullptr; } }
#define SAFE_DELETE_ARRAY(p)	{ if (p) { delete[] p;	p = nullptr; } }
#define SAFE_RELEASE(p)			{ if (p) { p->Release();p = nullptr; } }

#define FAILED_RETURN(f)		{ hr = f; if (FAILED( hr )) return hr; }