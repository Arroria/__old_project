#pragma once
struct POINT_ex
	: public POINT
{
	POINT_ex() { x = y = NULL; }
	POINT_ex(const long& size) { x = y = size; }
	POINT_ex(const long& _x, const long& _y) { x = _x; y = _y; }
	POINT_ex(const POINT_ex& _point) { x = _point.x; y = _point.y; }
	POINT_ex&	operator=(const POINT_ex& _point)	{ x = _point.x; y = _point.y; return *this; }
	bool		operator==(const POINT_ex& _point)	const { return x == _point.x && y == _point.y; }
	bool		operator!=(const POINT_ex& _point)	const { return x != _point.x && y != _point.y; }
	POINT_ex	operator+(const POINT_ex& _point)	const { return POINT_ex(x + _point.x, y + _point.y); }
	POINT_ex	operator-(const POINT_ex& _point)	const { return POINT_ex(x - _point.x, y - _point.y); }
	POINT_ex	operator+(const int& value)			const { return POINT_ex(x + value, y + value); }
	POINT_ex	operator-(const int& value)			const { return POINT_ex(x - value, y - value); }
	POINT_ex	operator*(const int& value)			const { return POINT_ex(x * value, y * value); }
	POINT_ex	operator/(const int& value)			const { return POINT_ex(x / value, y / value); }
	POINT_ex	operator*(const float& value)		const { return POINT_ex(x * value, y * value); }
	POINT_ex	operator/(const float& value)		const { return POINT_ex(x / value, y / value); }
	POINT_ex&	operator+=(const POINT_ex& _point)	{ x += _point.x;	y += _point.y;	return *this; }
	POINT_ex&	operator-=(const POINT_ex& _point)	{ x -= _point.x;	y -= _point.y;	return *this; }
	POINT_ex&	operator+=(const int& value)		{ x += value;		y += value;		return *this; }
	POINT_ex&	operator-=(const int& value)		{ x -= value;		y -= value;		return *this; }
	POINT_ex&	operator*=(const int& value)		{ x *= value;		y *= value;		return *this; }
	POINT_ex&	operator/=(const int& value)		{ x /= value;		y /= value;		return *this; }
	POINT_ex&	operator*=(const float& value)		{ x *= value;		y *= value;		return *this; }
	POINT_ex&	operator/=(const float& value)		{ x /= value;		y /= value;		return *this; }
};

struct RECT_ex
	: public RECT
{
	RECT_ex() { left = top = right = bottom = NULL; }
	RECT_ex(const long& _width, const long& _height) { left = -(right = _width * 0.5f); top = -(bottom = _height * 0.5f); }
	RECT_ex(const POINT_ex& _point) : RECT_ex(_point.x, _point.y) {}
	RECT_ex(const long& _left, const long& _top, const long& _right, const long& _bottom) { left = _left; top = _top; right = _right; bottom = _bottom; }
	RECT_ex(const RECT& rect) { left = rect.left; top = rect.top; right = rect.right; bottom = rect.bottom; }

	RECT_ex&	operator=(const RECT_ex& _rect)		{ left = _rect.left; top = _rect.top; right = _rect.right; bottom = _rect.bottom; return *this; }
	bool		operator==(const RECT_ex& _rect)	const { return left == _rect.left && top == _rect.top && right == _rect.right && bottom == _rect.bottom; }
	bool		operator!=(const RECT_ex& _rect)	const { return left != _rect.left && top != _rect.top && right != _rect.right && bottom == _rect.bottom; }
	RECT_ex		operator+(const RECT_ex& _rect)		const { return RECT_ex(left + _rect.left, top + _rect.top, right + _rect.right, bottom + _rect.bottom); }
	RECT_ex		operator-(const RECT_ex& _rect)		const { return RECT_ex(left - _rect.left, top - _rect.top, right - _rect.right, bottom - _rect.bottom); }
	RECT_ex		operator+(const POINT_ex& _point)	const { return RECT_ex(left + _point.x, top + _point.y, right + _point.x, bottom + _point.y); }
	RECT_ex		operator-(const POINT_ex& _point)	const { return RECT_ex(left - _point.x, top - _point.y, right - _point.x, bottom - _point.y); }
	RECT_ex		operator+(const int& value)			const { return RECT_ex(left + value, top + value, right + value, bottom + value); }
	RECT_ex		operator-(const int& value)			const { return RECT_ex(left - value, top - value, right - value, bottom - value); }
	RECT_ex		operator*(const int& value)			const { return RECT_ex(left * value, top * value, right * value, bottom * value); }
	RECT_ex		operator/(const int& value)			const { return RECT_ex(left / value, top / value, right / value, bottom / value); }
	RECT_ex		operator*(const float& value)		const { return RECT_ex(left * value, top * value, right * value, bottom * value); }
	RECT_ex		operator/(const float& value)		const { return RECT_ex(left / value, top / value, right / value, bottom / value); }
	RECT_ex&	operator+=(const RECT_ex& _rect)	{ left += _rect.left;	top += _rect.top;	right += _rect.right;	bottom += _rect.bottom;	return *this; }
	RECT_ex&	operator-=(const RECT_ex& _rect)	{ left -= _rect.left;	top -= _rect.top;	right -= _rect.right;	bottom -= _rect.bottom;	return *this; }
	RECT_ex&	operator+=(const POINT_ex& _point)	{ return RECT_ex(left += _point.x, top += _point.y, right += _point.x, bottom += _point.y); }
	RECT_ex&	operator-=(const POINT_ex& _point)	{ return RECT_ex(left -= _point.x, top -= _point.y, right -= _point.x, bottom -= _point.y); }
	RECT_ex&	operator+=(const int& value)		{ left += value;		top += value;		right += value;			bottom += value;		return *this; }
	RECT_ex&	operator-=(const int& value)		{ left -= value;		top -= value;		right -= value;			bottom -= value;		return *this; }
	RECT_ex&	operator*=(const int& value)		{ left *= value;		top *= value;		right *= value;			bottom *= value;		return *this; }
	RECT_ex&	operator/=(const int& value)		{ left /= value;		top /= value;		right /= value;			bottom /= value;		return *this; }
	RECT_ex&	operator*=(const float& value)		{ left *= value;		top *= value;		right *= value;			bottom *= value;		return *this; }
	RECT_ex&	operator/=(const float& value)		{ left /= value;		top /= value;		right /= value;			bottom /= value;		return *this; }
};