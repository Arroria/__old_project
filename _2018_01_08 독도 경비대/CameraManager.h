#pragma once

class Camera
{
private:
	//View
	D3DXVECTOR3 position;
	D3DXVECTOR3 at;
	D3DXVECTOR3 up;

	//Projection
	float	fov;
	float	aspect;
	float	nearRange;
	float	farRange;

	//save
	bool	changed;
	D3DXMATRIX view;
	D3DXMATRIX projection;

public:
	void SetCameraView		(const D3DXVECTOR3* _position, const D3DXVECTOR3* _at, const D3DXVECTOR3* _up);
	void SetCameraProjection(const float& _fov, const float& _aspect, const float& _nearRange, const float& _farRange);
	void SetCameraFOV		(const float& _fov)			{ fov		= _fov; }
	void SetCameraAspect	(const float& _aspect)		{ aspect	= _aspect; }
	void SetCameraNearRange	(const float& _nearRange)	{ nearRange	= _nearRange; }
	void SetCameraFarRange	(const float& _farRange)	{ farRange	= _farRange; }
	HRESULT	MatrixInput(LPDIRECT3DDEVICE9 device);

	D3DXVECTOR3 GetCameraPosition()	{ return position; }
	D3DXVECTOR3 GetCameraAt()		{ return at; }
	D3DXVECTOR3 GetCameraUp()		{ return up; }

public:
	Camera();
	~Camera();
};


class CameraManager
{
private:
	

public:
	CameraManager();
	~CameraManager();
};

