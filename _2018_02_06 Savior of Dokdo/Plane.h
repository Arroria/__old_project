#pragma once
#include "Singleton.h"
class Plane :
	public Singleton<Plane>
{
private:
	struct PlaneVertex
	{
		D3DXVECTOR3 p;
		D3DXVECTOR3 n;
		D3DXVECTOR2 t;
		PlaneVertex() {}
		PlaneVertex(
			const D3DXVECTOR3& _p,
			const D3DXVECTOR3& _n,
			const D3DXVECTOR2& _t)
			: p(_p)
			, n(_n)
			, t(_t)
		{}

		enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
	};
	PlaneVertex m_plane[6];

public:
	DWORD GetFVF() { return PlaneVertex::FVF; }
	void Render(LPDIRECT3DDEVICE9 device);

public:
	Plane();
	~Plane();
};

