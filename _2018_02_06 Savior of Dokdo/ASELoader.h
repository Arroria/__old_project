#pragma once
#include "Singleton.h"


struct Vertex_PNT
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 uv;

	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
};

typedef std::pair<D3DMATERIAL9, LPDIRECT3DTEXTURE9> MtrlTex;
struct GeomObj
{
	std::vector<Vertex_PNT> vertexData;
	MtrlTex* mtrlTex;
	D3DXMATRIX localMatrix;
	D3DXMATRIX localMatrixSR;
	D3DXMATRIX localMatrixT;
};

class ASELoader;
class ASE
{
public:
	friend ASELoader;

private:
	std::vector<GeomObj*> m_geomObj;
	std::vector<MtrlTex> m_mtrlTexList;

public:
	UINT		GetAttributeCount();
	GeomObj*	GetGeomObj(const UINT& attribute);

	void SetFVF			(LPDIRECT3DDEVICE9 device);
	void Render			(LPDIRECT3DDEVICE9 device, const UINT& attribute);

public:
	ASE();
	~ASE();
};


class ASELoader :
	public Singleton<ASELoader>
{
private:
	FILE* m_file;
	std::wstring m_word;
	std::wstring m_filePath;
	std::wstring m_fileName;

	ASE* m_ase;

public:
	ASE* LoadASE(const std::wstring& filePath, const std::wstring& fileName);
private:
	std::wstring&	NextWord();
	int				NextInt();
	float			NextFloat();

	void	Read_SCENE();
	void	Read_MATERIAL_LIST();
	void	Read_MATERIAL			(MtrlTex& mtrlTex);
	void	Read_MAP_DIFFUSE		(LPDIRECT3DTEXTURE9& texture);
	void	Read_GEOMOBJECT();
	void	Read_NODE_TM			(D3DXMATRIX& localMatrix, D3DXMATRIX& localMatrixSR, D3DXMATRIX& localMatrixT);
	void	Read_MESH				(GeomObj* geomObj);
	void	Read_MESH_VERTEX_LIST	(std::vector<D3DXVECTOR3>& position);
	void	Read_MESH_FACE_LIST		(std::vector<Vertex_PNT>& vertexData, std::vector<D3DXVECTOR3>& position);
	void	Read_MESH_TVERTLIST		(std::vector<D3DXVECTOR2>& uv);
	void	Read_MESH_TFACELIST		(std::vector<Vertex_PNT>& vertexData, std::vector<D3DXVECTOR2>& uv);
	void	Read_MESH_NORMALS		(std::vector<Vertex_PNT>& vertexData);

public:
	ASELoader();
	~ASELoader();
};

