#pragma once
#include "Singleton.h"

class ASEManager;
class ASE
{
	friend ASEManager;
private:
	struct ASEVertex
	{
		D3DXVECTOR3 p;
		D3DXVECTOR3 n;
		D3DXVECTOR2 t;

		ASEVertex() {}
		ASEVertex(const D3DXVECTOR3& _p, const D3DXVECTOR3& _n, const D3DXVECTOR2& _t)
			: p(_p), n(_n), t(_t) {}

		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1
		};
	};
	std::vector<ASEVertex> m_vData;
	LPDIRECT3DTEXTURE9 m_tex;
	D3DMATERIAL9 m_mtrl;

public:
	void Render();

public:
	ASE();
	~ASE();
};

class ASEManager :
	public Singleton<ASEManager>
{
private:
	std::map<std::wstring, ASE*> m_aseList;

public:
	ASE* LoadASE(const std::wstring& path, const std::wstring& name, const std::wstring& key);
	ASE* GetASE(const std::wstring& key);

public:
	ASEManager();
	~ASEManager();
};

#define ASELoad(path, name, key)	(SingletonInstance(ASEManager)->LoadASE(path, name, key))
#define ASEGet(key)					(SingletonInstance(ASEManager)->GetASE(key))
