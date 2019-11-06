#pragma once
#include "Singleton.h"
class Plane :
	public Singleton<Plane>
{
	struct PlaneVertex
	{
		D3DXVECTOR3 p;
		D3DXVECTOR3 n;
		D3DXVECTOR2 t;

		PlaneVertex() {}
		PlaneVertex(const D3DXVECTOR3& _p, const D3DXVECTOR3& _n, const D3DXVECTOR2& _t)
			: p(_p), n(_n), t(_t) {}

		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1
		};
	};
private:
	PlaneVertex m_planeData[4];

public:
	static D3DXMATRIX PivotMatrix(const D3DXVECTOR2& pivot);
	static D3DXMATRIX PivotMatrix(const D3DXVECTOR3& pivot);
	static DWORD GetFVF();
	HRESULT Render (LPDIRECT3DDEVICE9 device) const;

public:
	Plane();
	~Plane();
};

