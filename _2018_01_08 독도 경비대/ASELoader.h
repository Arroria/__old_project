#pragma once

struct ASEVertex
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 uv;
	enum
	{
		FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1
	};
};

class ASEGeometry
{
private:
	std::wstring	m_name;

	D3DXMATRIX		m_matrix;
	std::vector<ASEVertex>*	m_vertex;
	std::pair< LPDIRECT3DTEXTURE9, D3DMATERIAL9 >* m_material;


public:
	void SetName(const std::wstring& name)			{ m_name = name; }
	void SetMatrix(const D3DXMATRIX& matrix)		{ m_matrix = matrix; }
	void SetVertex(std::vector<ASEVertex>* vertex)	{ m_vertex = vertex; }
	void SetMaterial(std::pair< LPDIRECT3DTEXTURE9, D3DMATERIAL9 >* material) { m_material = material; }

	std::wstring GetName() { return m_name; }

public:
	void Render(const D3DXMATRIX& world);

};

class ASE
{
private:
	std::map<std::wstring, ASEGeometry*> m_geometryList;
	std::vector<std::pair< LPDIRECT3DTEXTURE9, D3DMATERIAL9 >> m_material;

public:
	void InsertGeometry(ASEGeometry* geometry);
	void SetMaterialListSize(const UINT& size);
	void SetMaterial(const UINT& index, const std::pair<LPDIRECT3DTEXTURE9, D3DMATERIAL9>& material);

	std::pair<LPDIRECT3DTEXTURE9, D3DMATERIAL9>* GetMaterial(const UINT& index);

public:
	void Render(const D3DXMATRIX& world);

};

class ASELoader
{
private:
	

//Load
public:
	ASE* LoadASE(const std::wstring& fileFolder, const std::wstring& fileName);

private:
	FILE*			m_ase;
	std::wstring	m_aseFolder;
	std::wstring	m_aseName;
	std::wstring	m_word;

	std::wstring&	GetWord();
	int				GetInteger();
	float			GetFloat();

	ASE*	AnalysisASE();

	void	AnalysisMaterialList(ASE* ase);
	std::pair<LPDIRECT3DTEXTURE9, D3DMATERIAL9>	AnalysisMaterial();
	LPDIRECT3DTEXTURE9	AnalysisMaterialTexture();

	ASEGeometry*			AnalysisGeomety(ASE* ase);
	D3DXMATRIX				AnalysisGeomTM();
	std::vector<ASEVertex>*	AnalysisGeomMesh();

	void AnalysisGeomMeshVertexPositionRead	(std::vector<D3DXVECTOR3>& positionData);
	void AnalysisGeomMeshVertexPositionWrite(std::vector<D3DXVECTOR3>& positionData, std::vector<ASEVertex>* result);
	void AnalysisGeomMeshVertexUVRead		(std::vector<D3DXVECTOR2>& uvData);
	void AnalysisGeomMeshVertexUVWrite		(std::vector<D3DXVECTOR2>& uvData, std::vector<ASEVertex>* result);


public:
	ASELoader();
	~ASELoader();
};

