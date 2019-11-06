#include "stdafx.h"
#include "ASE.h"


#define g_Device g_processManager->GetDevice()


ASE::ASE()
{
}
ASE::~ASE()
{
}


void ASE::Render(const D3DXMATRIX& matrix)
{
	for (auto& iter : headGeometryObjContainer)
		iter->Render(matrix);
}



void GEOMETRYOBJ::Render(const D3DXMATRIX& matrix)
{
	g_Device->SetFVF(TEMP_VERTEX::FVF);
	g_Device->SetTexture(0, mtrltex.texture);
	g_Device->SetMaterial(&mtrltex.material);
	g_Device->SetTransform(D3DTS_WORLD, &(/*objMatrix * */matrix));
	g_Device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, vertexData.size() / 3, &(vertexData[0]), sizeof(TEMP_VERTEX));
	g_Device->SetTexture(0, nullptr);
}



std::wstring & ASELoader::GetData()
{
	dataFromFile.clear();
	bool isString(false);
	while (!feof(aseFile))
	{
		WCHAR dataPiece(getwc(aseFile));

		if (dataPiece == '\"')
		{
			if (isString)
				break;
			isString = true;
			continue;
		}

		if (dataPiece <= 32)
		{
			if (!isString)
			{
				if (dataFromFile.empty())
					continue;
				break;
			}
		}

		dataFromFile += dataPiece;
	}
	///wcout << dataFromFile << endl;
	return dataFromFile;
}

int ASELoader::GetDataInteger()
{
	return _wtoi(GetData().c_str());
}

float ASELoader::GetDataFloat()
{
	return _wtof(GetData().c_str());
}


void ASELoader::ReadArea_MATERIAL_LIST(std::vector<MTRLTEX>* mtrltexContainer)
{
	int level(0);
	do
	{
		GetData();
		if (dataFromFile == L"{") level++;
		else if (dataFromFile == L"}") level--;

		else if (dataFromFile == L"*MATERIAL_COUNT") mtrltexContainer->resize(GetDataInteger());
		else if (dataFromFile == L"*MATERIAL") ReadArea_MATERIAL(&(*mtrltexContainer)[GetDataInteger()]);
	} while (level > 0);
}

void ASELoader::ReadArea_MATERIAL(MTRLTEX* mtrltex)
{
	int level(0);
	do
	{
		GetData();
		if (dataFromFile == L"{") level++;
		else if (dataFromFile == L"}") level--;

		else if (dataFromFile == L"*MATERIAL_AMBIENT")
		{
			D3DCOLORVALUE& ambient = mtrltex->material.Ambient;
			ambient.r = GetDataFloat();
			ambient.g = GetDataFloat();
			ambient.b = GetDataFloat();
			ambient.a = 1;
		}
		else if (dataFromFile == L"*MATERIAL_DIFFUSE")
		{
			D3DCOLORVALUE& diffuse = mtrltex->material.Diffuse;
			diffuse.r = GetDataFloat();
			diffuse.g = GetDataFloat();
			diffuse.b = GetDataFloat();
			diffuse.a = 1;
		}
		else if (dataFromFile == L"*MATERIAL_SPECULAR")
		{
			D3DCOLORVALUE& specular = mtrltex->material.Specular;
			specular.r = GetDataFloat();
			specular.g = GetDataFloat();
			specular.b = GetDataFloat();
			specular.a = 1;
		}
		else if (dataFromFile == L"*MAP_DIFFUSE")
			ReadArea_MAP_DIFFUSE(&mtrltex->texture);
	} while (level > 0);
}

void ASELoader::ReadArea_MAP_DIFFUSE(LPDIRECT3DTEXTURE9 * texture)
{
	int level(0);
	do
	{
		GetData();
		if (dataFromFile == L"{") level++;
		else if (dataFromFile == L"}") level--;

		else if (dataFromFile == L"*BITMAP")
		{
			GetData();
			g_textureContainer->LoadTexture(dataFromFile, dataFromFile);
			*texture = g_textureContainer->GetTexture(dataFromFile)->d3dxTexture;
			/// if (dataFromFile.find(':') != std::wstring::npos)
			/// else
			/// 	;//±ÍÂú¾Æ
			/// // g_textureContainer->LoadTexture(dataFromFile, dataFromFile);
		}
	} while (level > 0);
}

GEOMETRYOBJ* ASELoader::ReadArea_GEOMOBJECT(std::vector<GEOMETRYOBJ*>* headGeometryObjContainer, std::map<std::wstring, GEOMETRYOBJ*>* geometryObjContainer, std::vector<MTRLTEX>* mtrltexContainer)
{
	GEOMETRYOBJ* geometryObj = new GEOMETRYOBJ();
	bool isHaveParent(false);

	int level(0);
	do
	{
		GetData();
		if (dataFromFile == L"{") level++;
		else if (dataFromFile == L"}") level--;

		else if (dataFromFile == L"*NODE_NAME") geometryObjContainer->insert(std::make_pair(dataFromFile, geometryObj));
		else if (dataFromFile == L"*NODE_PARENT")
		{
			isHaveParent = true;
		}
		else if (dataFromFile == L"*NODE_TM") ReadArea_NODE_TM(&geometryObj->objMatrix);
		else if (dataFromFile == L"*MESH") ReadArea_MESH(&geometryObj->vertexData);
		else if (dataFromFile == L"*MATERIAL_REF") geometryObj->mtrltex = (*mtrltexContainer)[GetDataInteger()];
	} while (level > 0);

	if (!isHaveParent)
		headGeometryObjContainer->push_back(geometryObj);
	return geometryObj;
}

void ASELoader::ReadArea_NODE_TM(D3DXMATRIX* matrix)
{
	int level(0);
	do
	{
		GetData();
		if (dataFromFile == L"{") level++;
		else if (dataFromFile == L"}") level--;

		else if (dataFromFile == L"*TM_ROW0")
		{
			matrix->_11	= GetDataFloat();
			matrix->_13	= GetDataFloat();
			matrix->_12	= GetDataFloat();
			matrix->_14 = 0;
		}
		else if (dataFromFile == L"*TM_ROW1")
		{
			matrix->_31 = GetDataFloat();
			matrix->_33 = GetDataFloat();
			matrix->_32 = GetDataFloat();
			matrix->_34 = 0;
		}
		else if (dataFromFile == L"*TM_ROW2")
		{
			matrix->_21 = GetDataFloat();
			matrix->_23 = GetDataFloat();
			matrix->_22 = GetDataFloat();
			matrix->_24 = 0;
		}
		else if (dataFromFile == L"*TM_ROW3")
		{
			matrix->_41 = GetDataFloat();
			matrix->_43 = GetDataFloat();
			matrix->_42 = GetDataFloat();
			matrix->_44 = 1;
		}
	} while (level > 0);
}

void ASELoader::ReadArea_MESH(std::vector<TEMP_VERTEX>* vertexData)
{
	std::vector<D3DXVECTOR3> positionVertex;
	std::vector<D3DXVECTOR2> uvVertex;
	std::vector<D3DXVECTOR3> normalVertex;

	int level(0);
	do
	{
		GetData();
		if (dataFromFile == L"{") level++;
		else if (dataFromFile == L"}") level--;

		else if (dataFromFile == L"*MESH_NUMVERTEX") positionVertex.resize(GetDataInteger());
		else if (dataFromFile == L"*MESH_NUMFACES") vertexData->resize(GetDataInteger() * 3);
		else if (dataFromFile == L"*MESH_VERTEX_LIST") ReadArea_MESH_VERTEX_LIST(&positionVertex);
		else if (dataFromFile == L"*MESH_FACE_LIST") ReadArea_MESH_FACE_LIST(&positionVertex, vertexData);
		else if (dataFromFile == L"*MESH_NUMTVERTEX") uvVertex.resize(GetDataInteger());
		else if (dataFromFile == L"*MESH_TVERTLIST") ReadArea_MESH_TVERTLIST(&uvVertex);
		else if (dataFromFile == L"*MESH_TFACELIST") ReadArea_MESH_TFACELIST(&uvVertex, vertexData);
		else if (dataFromFile == L"*MESH_NORMALS") ReadArea_MESH_NORMALS(vertexData);
	} while (level > 0);

	
}

void ASELoader::ReadArea_MESH_VERTEX_LIST(std::vector<D3DXVECTOR3>* positionVertex)
{
	int level(0);
	do
	{
		GetData();
		if (dataFromFile == L"{") level++;
		else if (dataFromFile == L"}") level--;

		else if (dataFromFile == L"*MESH_VERTEX")
		{
			int index = GetDataInteger();
			D3DXVECTOR3 position;
			position.x = GetDataFloat();
			position.z = GetDataFloat();
			position.y = GetDataFloat();
			(*positionVertex)[index] = position;
		}
	} while (level > 0);
}

void ASELoader::ReadArea_MESH_FACE_LIST(std::vector<D3DXVECTOR3>* positionVertex, std::vector<TEMP_VERTEX>* vertexData)
{
	int level(0);
	do
	{
		GetData();
		if (dataFromFile == L"{") level++;
		else if (dataFromFile == L"}") level--;

		else if (dataFromFile == L"*MESH_FACE")
		{
			int index = GetDataInteger() * 3;
			GetData();	(*vertexData)[index].position = (*positionVertex)[GetDataInteger()];
			GetData();	(*vertexData)[index + 2].position = (*positionVertex)[GetDataInteger()];
			GetData();	(*vertexData)[index + 1].position = (*positionVertex)[GetDataInteger()];
		}
	} while (level > 0);
}

void ASELoader::ReadArea_MESH_TVERTLIST(std::vector<D3DXVECTOR2>* uvVertex)
{
	int level(0);
	do
	{
		GetData();
		if (dataFromFile == L"{") level++;
		else if (dataFromFile == L"}") level--;

		else if (dataFromFile == L"*MESH_TVERT")
		{
			int index = GetDataInteger();
			D3DXVECTOR2 uv;
			uv.x = GetDataFloat();
			uv.y = 1 - GetDataFloat();
			(*uvVertex)[index] = uv;
		}
	} while (level > 0);
}

void ASELoader::ReadArea_MESH_TFACELIST(std::vector<D3DXVECTOR2>* uvVertex, std::vector<TEMP_VERTEX>* vertexData)
{
	int level(0);
	do
	{
		GetData();
		if (dataFromFile == L"{") level++;
		else if (dataFromFile == L"}") level--;

		else if (dataFromFile == L"*MESH_TFACE")
		{
			int index = GetDataInteger() * 3;
			(*vertexData)[index].uv = (*uvVertex)[GetDataInteger()];
			(*vertexData)[index + 2].uv = (*uvVertex)[GetDataInteger()];
			(*vertexData)[index + 1].uv = (*uvVertex)[GetDataInteger()];
		}
	} while (level > 0);
}

void ASELoader::ReadArea_MESH_NORMALS(std::vector<TEMP_VERTEX>* vertexData)
{
	int index = NULL;
	int readCount = NULL;
	const int correction[3] = { 0, 2, 1 };

	int level(0);
	do
	{
		GetData();
		if (dataFromFile == L"{") level++;
		else if (dataFromFile == L"}") level--;

		else if (dataFromFile == L"*MESH_FACENORMAL")
		{
			index = GetDataInteger() * 3;
			readCount = 0;
		}
		else if (dataFromFile == L"*MESH_VERTEXNORMAL")
		{
			D3DXVECTOR3 normal;
			normal.x = GetDataFloat();
			normal.z = GetDataFloat();
			normal.y = GetDataFloat();
			(*vertexData)[index + correction[readCount]].normal = normal;
			readCount++;
		}
	} while (level > 0);
}

ASE * ASELoader::FileLoad(const std::wstring & path)
{
	aseFile = _wfopen(path.c_str(), L"r");
	if (!aseFile)
		return nullptr;

	ASE* ase = new ASE();

	while (true)
	{
		if (GetData().empty())
			break;

			 if (dataFromFile == L"*MATERIAL_LIST") ReadArea_MATERIAL_LIST(&ase->mtrltexContainer);
		else if (dataFromFile == L"*GEOMOBJECT") ReadArea_GEOMOBJECT(&ase->headGeometryObjContainer, &ase->geometryObjContainer, &ase->mtrltexContainer);
	}
	return ase;
}





void ASELoader::RegisterAse(const std::wstring & key, const std::wstring & path)
{
	ase.insert(std::make_pair(key, this->FileLoad(path)));
}

ASE * ASELoader::GetAse(const std::wstring & key)
{
	return ase[key];
}
