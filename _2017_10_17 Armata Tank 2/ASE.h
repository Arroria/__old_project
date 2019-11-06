#pragma once
class ASELoader;

struct MTRLTEX
{
	LPDIRECT3DTEXTURE9 texture;
	D3DMATERIAL9 material;
};
struct TEMP_VERTEX
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 uv;
	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
};
struct GEOMETRYOBJ
{
	D3DXMATRIX objMatrix;
	MTRLTEX mtrltex;
	std::vector<TEMP_VERTEX> vertexData;

	void Render(const D3DXMATRIX& matrix);
};

class ASE
{
friend ASELoader;
private:
	std::vector<MTRLTEX> mtrltexContainer;
	std::vector<GEOMETRYOBJ*> headGeometryObjContainer;
	std::map<std::wstring, GEOMETRYOBJ*> geometryObjContainer;

public:
	void Render(const D3DXMATRIX& matrix);

public:
	ASE();
	~ASE();
};

class ASELoader
{
private:
	FILE* aseFile;
	std::wstring aseFilePath;

	std::wstring dataFromFile;

private:
	std::wstring&	GetData();
	int				GetDataInteger();
	float			GetDataFloat();

private:
	void ReadArea_MATERIAL_LIST(std::vector<MTRLTEX>* mtrltexContainer);
	void ReadArea_MATERIAL(MTRLTEX* mtrltex);
	void ReadArea_MAP_DIFFUSE(LPDIRECT3DTEXTURE9* texture);
	
	GEOMETRYOBJ*  ReadArea_GEOMOBJECT(std::vector<GEOMETRYOBJ*>* headGeometryObjContainer, std::map<std::wstring, GEOMETRYOBJ*>* geometryObjContainer, std::vector<MTRLTEX>* mtrltexContainer);
	void ReadArea_NODE_TM(D3DXMATRIX* matrix);
	void ReadArea_MESH(std::vector<TEMP_VERTEX>* vertexData);

	void ReadArea_MESH_VERTEX_LIST(std::vector<D3DXVECTOR3>* positionVertex);
	void ReadArea_MESH_FACE_LIST(std::vector<D3DXVECTOR3>* positionVertex, std::vector<TEMP_VERTEX>* vertexData);
	void ReadArea_MESH_TVERTLIST(std::vector<D3DXVECTOR2>* uvVertex);
	void ReadArea_MESH_TFACELIST(std::vector<D3DXVECTOR2>* uvVertex, std::vector<TEMP_VERTEX>* vertexData);
	void ReadArea_MESH_NORMALS(std::vector<TEMP_VERTEX>* vertexData);

public:
	ASE* FileLoad(const std::wstring& path);

	//temp
private:
	std::map<std::wstring, ASE*> ase;

public:
	void RegisterAse(const std::wstring& key, const std::wstring& path);
	ASE* GetAse(const std::wstring& key);

public:
	ASELoader() {}
	~ASELoader() {}
};

