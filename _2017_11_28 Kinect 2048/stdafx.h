#pragma once
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//Library
#include <iostream>
using std::cout;
using std::wcout;
using std::endl;

#include <windowsx.h>
#include <string>
#include <vector>
#include <map>
#include <list>

#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3d9.h"
#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3dx9.h"
#pragma comment (lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/lib/x86/d3d9.lib")
#pragma comment (lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/lib/x86/d3dx9.lib")



//Kinect
#include <Kinect.h>
#pragma comment (lib, "C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409/Lib/x86/Kinect20.lib")

#include "KinectManager.h"
#include "HandInputCalculator.h"
extern KinectManager* g_kinectManager;
extern HandInputCalculator* g_handInputCalculator;


//Default struct/class extend
struct POINT_ex
	: public POINT
{
	POINT_ex() { x = y = NULL; }
	POINT_ex(const long& size) { x = y = size; }
	POINT_ex(const long& _x, const long& _y) { x = _x; y = _y; }
	POINT_ex(const POINT& _point) { x = _point.x; y = _point.y; }
	POINT_ex& operator=(const POINT& _point) { x = _point.x; y = _point.y; return *this; }
	bool operator==(const POINT& _point) { return x == _point.x && y == _point.y; }
	bool operator!=(const POINT& _point) { return x != _point.x && y != _point.y; }
	POINT_ex operator+(const POINT& _point)	{ return POINT_ex(x + _point.x, y + _point.y); }
	POINT_ex operator-(const POINT& _point)	{ return POINT_ex(x - _point.x, y - _point.y); }
	POINT_ex operator+(const int& value)	{ return POINT_ex(x + value, y + value); }
	POINT_ex operator-(const int& value)	{ return POINT_ex(x - value, y - value); }
	POINT_ex operator*(const int& value)	{ return POINT_ex(x * value, y * value); }
	POINT_ex operator/(const int& value)	{ return POINT_ex(x / value, y / value); }
	POINT_ex operator*(const float& value)	{ return POINT_ex(x * value, y * value); }
	POINT_ex operator/(const float& value)	{ return POINT_ex(x / value, y / value); }
	POINT_ex& operator+=(const POINT& _point)	{ x += _point.x;	y += _point.y;	return *this; }
	POINT_ex& operator-=(const POINT& _point)	{ x -= _point.x;	y -= _point.y;	return *this; }
	POINT_ex& operator+=(const int& value)		{ x += value;		y += value;		return *this; }
	POINT_ex& operator-=(const int& value)		{ x -= value;		y -= value;		return *this; }
	POINT_ex& operator*=(const int& value)		{ x *= value;		y *= value;		return *this; }
	POINT_ex& operator/=(const int& value)		{ x /= value;		y /= value;		return *this; }
	POINT_ex& operator*=(const float& value)	{ x *= value;		y *= value;		return *this; }
	POINT_ex& operator/=(const float& value)	{ x /= value;		y /= value;		return *this; }
};
struct RECT_ex
	: public RECT
{
	RECT_ex() { left = top = right = bottom = NULL; }
	RECT_ex(const long& _width, const long& _height) { left = -(right = _width * 0.5f); top = -(bottom = _height * 0.5f); }
	RECT_ex(const POINT& _point) : RECT_ex(_point.x, _point.y) {}
	RECT_ex(const long& _left, const long& _top, const long& _right, const long& _bottom) { left = _left; top = _top; right = _right; bottom = _bottom; }
	RECT_ex(const RECT& rect) { left = rect.left; top = rect.top; right = rect.right; bottom = rect.bottom; }
	RECT_ex& operator=(const RECT_ex& _rect) { left = _rect.left; top = _rect.top; right = _rect.right; bottom = _rect.bottom; return *this; }
	bool operator==(const RECT_ex& _rect) { return left == _rect.left && top == _rect.top && right == _rect.right && bottom == _rect.bottom; }
	bool operator!=(const RECT_ex& _rect) { return left != _rect.left && top != _rect.top && right != _rect.right && bottom == _rect.bottom; }
	RECT_ex operator+(const RECT_ex& _rect)	{ return RECT_ex(left + _rect.left, top + _rect.top, right + _rect.right, bottom + _rect.bottom); }
	RECT_ex operator-(const RECT_ex& _rect)	{ return RECT_ex(left - _rect.left, top - _rect.top, right - _rect.right, bottom - _rect.bottom); }
	RECT_ex operator+(const POINT_ex& _point)	{ return RECT_ex(left + _point.x, top + _point.y, right + _point.x, bottom + _point.y); }
	RECT_ex operator-(const POINT_ex& _point)	{ return RECT_ex(left - _point.x, top - _point.y, right - _point.x, bottom - _point.y); }
	RECT_ex operator+(const int& value)		{ return RECT_ex(left + value, top + value, right + value, bottom + value); }
	RECT_ex operator-(const int& value)		{ return RECT_ex(left - value, top - value, right - value, bottom - value); }
	RECT_ex operator*(const int& value)		{ return RECT_ex(left * value, top * value, right * value, bottom * value); }
	RECT_ex operator/(const int& value)		{ return RECT_ex(left / value, top / value, right / value, bottom / value); }
	RECT_ex operator*(const float& value)	{ return RECT_ex(left * value, top * value, right * value, bottom * value); }
	RECT_ex operator/(const float& value)	{ return RECT_ex(left / value, top / value, right / value, bottom / value); }
	RECT_ex& operator+=(const RECT_ex& _rect)	{ left += _rect.left;	top += _rect.top;	right += _rect.right;	bottom += _rect.bottom;	return *this; }
	RECT_ex& operator-=(const RECT_ex& _rect)	{ left -= _rect.left;	top -= _rect.top;	right -= _rect.right;	bottom -= _rect.bottom;	return *this; }
	RECT_ex& operator+=(const POINT_ex& _point)	{ return RECT_ex(left += _point.x, top += _point.y, right += _point.x, bottom += _point.y); }
	RECT_ex& operator-=(const POINT_ex& _point)	{ return RECT_ex(left -= _point.x, top -= _point.y, right -= _point.x, bottom -= _point.y); }
	RECT_ex& operator+=(const int& value)		{ left += value;		top += value;		right += value;			bottom += value;		return *this; }
	RECT_ex& operator-=(const int& value)		{ left -= value;		top -= value;		right -= value;			bottom -= value;		return *this; }
	RECT_ex& operator*=(const int& value)		{ left *= value;		top *= value;		right *= value;			bottom *= value;		return *this; }
	RECT_ex& operator/=(const int& value)		{ left /= value;		top /= value;		right /= value;			bottom /= value;		return *this; }
	RECT_ex& operator*=(const float& value)		{ left *= value;		top *= value;		right *= value;			bottom *= value;		return *this; }
	RECT_ex& operator/=(const float& value)		{ left /= value;		top /= value;		right /= value;			bottom /= value;		return *this; }
};

//Define
#define SAFE_DELETE(p)			{ if (p) { delete p; p = nullptr; } }
#define SAFE_DELETE_ARRAY(p)	{ if (p) { delete[] p; p = nullptr; } }
#define SAFE_RELEASE(p)			{ if (p) { p->Release(); p = nullptr; } }




#include "ProcessManager.h"
extern ProcessManager* g_processManager;

#include "InputManager.h"
#include "SceneManager.h"
