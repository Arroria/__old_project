#include "stdafx.h"
#include "ASELoader.h"

using std::cout;
using std::wcout;
using std::endl;


LPDIRECT3DTEXTURE9 tx = nullptr;


void ASEGeometry::Render(const D3DXMATRIX& world)
{
	if (m_material)
	{
		DEVICE->SetTexture(0, m_material->first);
		DEVICE->SetMaterial(&m_material->second);
	}
	else
	{
		D3DMATERIAL9 defaultMaterial;
		ZeroMemory(&defaultMaterial, sizeof(defaultMaterial));
		defaultMaterial.Ambient = defaultMaterial.Diffuse = defaultMaterial.Specular = D3DXCOLOR(1, 1, 1, 1);
		DEVICE->SetTexture(0, nullptr);
		DEVICE->SetMaterial(&defaultMaterial);
	}
	DEVICE->SetTransform(D3DTS_WORLD, &world);
	DEVICE->SetFVF(ASEVertex::FVF);
	DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vertex->size() / 3, &(*m_vertex)[0], sizeof(ASEVertex));
}





void ASE::InsertGeometry(ASEGeometry * geometry)
{
	m_geometryList.insert(std::make_pair( geometry->GetName(), geometry ));
}

void ASE::SetMaterialListSize(const UINT & size)
{
	m_material.resize(size);
}

void ASE::SetMaterial(const UINT& index, const std::pair<LPDIRECT3DTEXTURE9, D3DMATERIAL9>& material)
{
	m_material[index] = material;
}

std::pair<LPDIRECT3DTEXTURE9, D3DMATERIAL9>* ASE::GetMaterial(const UINT & index)
{
	return &m_material[index];
}



void ASE::Render(const D3DXMATRIX& world)
{
	for (auto& iter : m_geometryList)
		iter.second->Render(world);
}





ASELoader::ASELoader()
{
}
ASELoader::~ASELoader()
{
}



ASE* ASELoader::LoadASE(const std::wstring & fileFolder, const std::wstring & fileName)
{
	m_aseFolder = fileFolder;
	m_aseName = fileName;
	m_ase = _wfopen((fileFolder + L"/" + fileName).c_str(), L"r");

	if (m_ase)
		return AnalysisASE();
	else
	{
		#if _DEBUG
		cout << "Can not found \" ";
		wcout << (fileFolder + L"/" + fileName);
		cout << "\" File." << endl;
		#endif
		return nullptr;
	}
}



std::wstring & ASELoader::GetWord()
{
	bool isString(false);


	m_word.clear();
	while (!feof(m_ase))
	{
		wchar_t character = fgetwc(m_ase);
		
		if (character == '\"')
		{
			if (isString)
				break;
			isString = true;
			continue;
		}

		if (character <= 32)
		{
			if (!isString)
			{
				if (m_word.empty())
					continue;
				break;
			}
		}

		m_word += character;
	}

	return m_word;
}

int ASELoader::GetInteger()
{
	return _wtoi(GetWord().c_str());
}

float ASELoader::GetFloat()
{
	return _wtof(GetWord().c_str());
}



ASE* ASELoader::AnalysisASE()
{
	ASE* ase = new ASE();

	while (!GetWord().empty())
	{
			 if (m_word == L"*SCENE")			continue;
		else if (m_word == L"*MATERIAL_LIST")	AnalysisMaterialList(ase);
		else if (m_word == L"*GEOMOBJECT")		ase->InsertGeometry(AnalysisGeomety(ase));
	}

	return ase;
}

void ASELoader::AnalysisMaterialList(ASE * ase)
{
	int depthLevel = 0;
	do {
		GetWord();

		if (m_word == L"{") depthLevel++;
		else if (m_word == L"}") depthLevel--;

		else if (m_word == L"*MATERIAL_COUNT")	ase->SetMaterialListSize(GetInteger());
		else if (m_word == L"*MATERIAL")		ase->SetMaterial(GetInteger(), AnalysisMaterial());
	} while (depthLevel);
}

std::pair<LPDIRECT3DTEXTURE9, D3DMATERIAL9> ASELoader::AnalysisMaterial()
{
	std::pair<LPDIRECT3DTEXTURE9, D3DMATERIAL9> material;
	ZeroMemory(&material.second, sizeof(D3DMATERIAL9));
	int depthLevel = 0;
	do {
		GetWord();

		if (m_word == L"{") depthLevel++;
		else if (m_word == L"}") depthLevel--;

		else if (m_word == L"*MATERIAL_AMBIENT")	{ material.second.Ambient.a = 1 ;	material.second.Ambient.r = GetFloat() ;	material.second.Ambient.g = GetFloat() ;	material.second.Ambient.b = GetFloat(); }
		else if (m_word == L"*MATERIAL_DIFFUSE")	{ material.second.Diffuse.a = 1 ;	material.second.Diffuse.r = GetFloat() ;	material.second.Diffuse.g = GetFloat() ;	material.second.Diffuse.b = GetFloat(); }
		else if (m_word == L"*MATERIAL_SPECULAR")	{ material.second.Specular.a = 1;	material.second.Specular.r = GetFloat();	material.second.Specular.g = GetFloat();	material.second.Specular.b = GetFloat(); }
		else if (m_word == L"*MAP_DIFFUSE")			{ material.first = AnalysisMaterialTexture(); }
	} while (depthLevel);

	return material;
}

LPDIRECT3DTEXTURE9 ASELoader::AnalysisMaterialTexture()
{
	LPDIRECT3DTEXTURE9 texture = nullptr;

	int depthLevel = 0;
	do {
		GetWord();

		if (m_word == L"{") depthLevel++;
		else if (m_word == L"}") depthLevel--;

		else if (m_word == L"*BITMAP")
		{
			std::wstring temp = m_aseFolder + L"/" + GetWord();
			D3DXCreateTextureFromFileExW(g_processManager->GetDevice(),
				(temp).c_str(),
				D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,
				1,
				NULL,
				D3DFMT_UNKNOWN,
				D3DPOOL_MANAGED,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				NULL,//0x00ffffff,
				nullptr,
				nullptr,
				&texture);
		}
	} while (depthLevel);
	return texture;
}

ASEGeometry * ASELoader::AnalysisGeomety(ASE* ase)
{
	ASEGeometry* geometry = new ASEGeometry();

	int depthLevel = 0;
	do {
		GetWord();

			 if (m_word == L"{") depthLevel++;
		else if (m_word == L"}") depthLevel--;

		else if (m_word == L"*NODE_NAME")		geometry->SetName(GetWord());
		else if (m_word == L"*NODE_TM")			geometry->SetMatrix(AnalysisGeomTM());
		else if (m_word == L"*MESH")			geometry->SetVertex(AnalysisGeomMesh());
		else if (m_word == L"*MATERIAL_REF")	geometry->SetMaterial(ase->GetMaterial(GetInteger()));
	} while (depthLevel);

	return geometry;
}

D3DXMATRIX ASELoader::AnalysisGeomTM()
{
	D3DXMATRIX transformMartrix;

	int depthLevel = 0;
	do {
		GetWord();

			 if (m_word == L"{") depthLevel++;
		else if (m_word == L"}") depthLevel--;

		else if (m_word == L"*TM_ROW0") { transformMartrix._11 = GetFloat(); transformMartrix._13 = GetFloat(); transformMartrix._12 = GetFloat(); transformMartrix._14 = 0; }
		else if (m_word == L"*TM_ROW2") { transformMartrix._31 = GetFloat(); transformMartrix._33 = GetFloat(); transformMartrix._32 = GetFloat(); transformMartrix._34 = 0; }
		else if (m_word == L"*TM_ROW1") { transformMartrix._21 = GetFloat(); transformMartrix._23 = GetFloat(); transformMartrix._22 = GetFloat(); transformMartrix._24 = 0; }
		else if (m_word == L"*TM_ROW3") { transformMartrix._41 = GetFloat(); transformMartrix._43 = GetFloat(); transformMartrix._42 = GetFloat(); transformMartrix._44 = 1; }
	} while (depthLevel);

	return transformMartrix;
}

std::vector<ASEVertex>* ASELoader::AnalysisGeomMesh()
{
	std::vector<ASEVertex>* result = new std::vector<ASEVertex>;
	std::vector<D3DXVECTOR3> positionData;
	std::vector<D3DXVECTOR2> uvData;


	int depthLevel = 0;
	do {
		GetWord();

			 if (m_word == L"{") depthLevel++;
		else if (m_word == L"}") depthLevel--;

		else if (m_word == L"*MESH_NUMVERTEX")	positionData.resize(GetInteger());
		else if (m_word == L"*MESH_NUMFACES")	result->resize(GetInteger() * 3);
		else if (m_word == L"*MESH_NUMTVERTEX")	uvData.resize(GetInteger());
		else if (m_word == L"*MESH_VERTEX_LIST")	AnalysisGeomMeshVertexPositionRead(positionData);
		else if (m_word == L"*MESH_FACE_LIST")		AnalysisGeomMeshVertexPositionWrite(positionData, result);
		else if (m_word == L"*MESH_TVERTLIST")		AnalysisGeomMeshVertexUVRead(uvData);
		else if (m_word == L"*MESH_TFACELIST")		AnalysisGeomMeshVertexUVWrite(uvData, result);
	} while (depthLevel);

	for (UINT faceIndex = 0; faceIndex < result->size(); faceIndex += 3)
	{
		D3DXVECTOR3 normal;
		D3DXVec3Cross(&normal,
			&((*result)[faceIndex + 1].position - (*result)[faceIndex].position),
			&((*result)[faceIndex + 2].position - (*result)[faceIndex].position));
		D3DXVec3Normalize(&normal, &normal);
		(*result)[faceIndex + 0].normal =
		(*result)[faceIndex + 1].normal =
		(*result)[faceIndex + 2].normal = normal;
	}

	return result;
}

void ASELoader::AnalysisGeomMeshVertexPositionRead(std::vector<D3DXVECTOR3>& positionData)
{
	int depthLevel = 0;
	do {
		GetWord();

			 if (m_word == L"{") depthLevel++;
		else if (m_word == L"}") depthLevel--;

		else if (m_word == L"*MESH_VERTEX")
		{
			D3DXVECTOR3& dataPlace = positionData[GetInteger()];
			dataPlace.x = GetFloat();
			dataPlace.z = GetFloat();
			dataPlace.y = GetFloat();
		}
	} while (depthLevel);
}

void ASELoader::AnalysisGeomMeshVertexPositionWrite(std::vector<D3DXVECTOR3>& positionData, std::vector<ASEVertex>* result)
{
	int depthLevel = 0;
	do {
		GetWord();

			 if (m_word == L"{") depthLevel++;
		else if (m_word == L"}") depthLevel--;

		else if (m_word == L"*MESH_FACE")
		{
			UINT dataPlace = GetInteger() * 3;
			GetWord();	(*result)[dataPlace + 0].position = positionData[GetInteger()];
			GetWord();	(*result)[dataPlace + 2].position = positionData[GetInteger()];
			GetWord();	(*result)[dataPlace + 1].position = positionData[GetInteger()];
		}
	} while (depthLevel);
}

void ASELoader::AnalysisGeomMeshVertexUVRead(std::vector<D3DXVECTOR2>& uvData)
{
	int depthLevel = 0;
	do {
		GetWord();

			 if (m_word == L"{") depthLevel++;
		else if (m_word == L"}") depthLevel--;

		else if (m_word == L"*MESH_TVERT")
		{
			D3DXVECTOR2& dataPlace = uvData[GetInteger()];
			dataPlace.x = GetFloat();
			dataPlace.y = 1.f - GetFloat();
		}
	} while (depthLevel);
}

void ASELoader::AnalysisGeomMeshVertexUVWrite(std::vector<D3DXVECTOR2>& uvData, std::vector<ASEVertex>* result)
{
	int depthLevel = 0;
	do {
		GetWord();

		if (m_word == L"{") depthLevel++;
		else if (m_word == L"}") depthLevel--;

		else if (m_word == L"*MESH_TFACE")
		{
			UINT dataPlace = GetInteger() * 3;
			(*result)[dataPlace + 0].uv = uvData[GetInteger()];
			(*result)[dataPlace + 2].uv = uvData[GetInteger()];
			(*result)[dataPlace + 1].uv = uvData[GetInteger()];
		}
	} while (depthLevel);
}
