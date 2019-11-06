#pragma once
#include "Singleton.h"

class ASEManager;
class EasyASE
{
	friend ASEManager;
private:
	struct ASEVertex
	{
		D3DXVECTOR3 p;
		D3DXVECTOR3 n;
		D3DXVECTOR2 t;
		enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };

		ASEVertex() {}
		ASEVertex(const D3DXVECTOR3& _p, const D3DXVECTOR3& _n, const D3DXVECTOR2& _t) : p(_p), n(_n), t(_t) {}
	};

	D3DMATERIAL9 m_mtrl;
	LPDIRECT3DTEXTURE9 m_tex;
	std::vector<ASEVertex> m_vertex;
	
public:
	void Render();

	EasyASE() { ZeroMemory(this, sizeof(EasyASE)); }
};

class ASEManager :
	public Singleton<ASEManager>
{
private:
	std::map<std::wstring, EasyASE*> m_aseList;

	FILE* m_file;
	std::wstring m_data;
	EasyASE* m_ase;

	std::wstring m_path;
	std::wstring m_name;

private:
	std::wstring& NextToken();
	int NextInt();
	float NextFloat();

public:
	EasyASE* LoadASE(const std::wstring& path, const std::wstring& name, const std::wstring& key);
	EasyASE* GetASE(const std::wstring& key);

private:
	void MTRLTEX();
	void MESH();

public:
	ASEManager();
	~ASEManager();
};

#define ASELoad(path, name, key)	(SingletonInstance(ASEManager)->LoadASE(path, name, key))
#define ASEGet(key)					(SingletonInstance(ASEManager)->GetASE(key))
