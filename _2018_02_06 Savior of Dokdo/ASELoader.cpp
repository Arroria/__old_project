#include "stdafx.h"
#include "ASELoader.h"


ASELoader::ASELoader()
	: m_file(nullptr)

	, m_ase(nullptr)
{
}
ASELoader::~ASELoader()
{
}



ASE * ASELoader::LoadASE(const std::wstring & filePath, const std::wstring & fileName)
{
	m_file = nullptr;
	m_file = _wfopen((filePath + L'/' + fileName).c_str(), L"r");
	if (!m_file)
		return nullptr;

	m_filePath = filePath;
	m_fileName = fileName;
	m_ase = new ASE();

	this;
	do
	{
		NextWord();
		
			 if (m_word == L"*SCENE")			Read_SCENE();
		else if (m_word == L"*MATERIAL_LIST")	Read_MATERIAL_LIST();
		else if (m_word == L"*GEOMOBJECT")		Read_GEOMOBJECT();

	} while (!m_word.empty());

	fclose(m_file);

	//m_file	= nullptr;
	//m_ase	= nullptr;
	//m_word.clear();
	//m_filePath.clear();
	//m_fileName.clear();
	//
	return m_ase;
}


std::wstring & ASELoader::NextWord()
{
	m_word.clear();

	bool isString = false;
	while (!feof(m_file))
	{
		WCHAR spell = fgetwc(m_file);
		
		if (spell == '\"')
		{
			if (isString)
				break;
			isString = true;
			continue;
		}

		if (!isString && spell <= 32)
		{
			if (m_word.empty())
				continue;
			break;
		}

		m_word += spell;
	}

	return m_word;
}
int ASELoader::NextInt()		{ return _wtoi(NextWord().c_str()); }
float ASELoader::NextFloat()	{ return _wtof(NextWord().c_str()); }


void ASELoader::Read_SCENE()
{
	UINT depth = 0;
	do
	{
		NextWord();
			 if (m_word == L"{")	depth++;
		else if (m_word == L"}")	depth--;
	} while (depth > 0);
}

void ASELoader::Read_MATERIAL_LIST()
{
	std::vector<MtrlTex>& mtrlList = m_ase->m_mtrlTexList;

	UINT depth = 0;
	do
	{
		NextWord();
			 if (m_word == L"{")	depth++;
		else if (m_word == L"}")	depth--;

		else if (m_word == L"*MATERIAL_COUNT")	mtrlList.resize(NextInt());
		else if (m_word == L"*MATERIAL")		Read_MATERIAL(mtrlList[NextInt()]);


	} while (depth > 0);
}

void ASELoader::Read_MATERIAL(MtrlTex& mtrlTex)
{
	D3DMATERIAL9& mtrl = mtrlTex.first;
	LPDIRECT3DTEXTURE9& tex = mtrlTex.second;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	tex = nullptr;

	UINT depth = 0;
	do
	{
		NextWord();
		if (m_word == L"{")	depth++;
		else if (m_word == L"}")	depth--;
		
		else if (m_word == L"*MATERIAL_AMBIENT")	{ mtrl.Ambient.r = NextFloat();		mtrl.Ambient.g = NextFloat();	mtrl.Ambient.b = NextFloat();	mtrl.Ambient.a = 1; }
		else if (m_word == L"*MATERIAL_DIFFUSE")	{ mtrl.Diffuse.r = NextFloat();		mtrl.Diffuse.g = NextFloat();	mtrl.Diffuse.b = NextFloat();	mtrl.Diffuse.a = 1; }
		else if (m_word == L"*MATERIAL_SPECULAR")	{ mtrl.Specular.r = NextFloat();	mtrl.Specular.g = NextFloat();	mtrl.Specular.b = NextFloat();	mtrl.Specular.a = 1; }
		else if (m_word == L"*MAP_DIFFUSE")			Read_MAP_DIFFUSE(tex);

	} while (depth > 0);
}

void ASELoader::Read_MAP_DIFFUSE(LPDIRECT3DTEXTURE9& texture)
{
	UINT depth = 0;
	do
	{
		NextWord();
		if (m_word == L"{")	depth++;
		else if (m_word == L"}")	depth--;
		
		else if (m_word == L"*BITMAP")
		{
			D3DXCreateTextureFromFileExW(
				DEVICE,
				(m_filePath + L'/' + NextWord()).c_str(),
				D3DX_DEFAULT_NONPOW2,
				D3DX_DEFAULT_NONPOW2,
				1,
				NULL,
				D3DFMT_UNKNOWN,
				D3DPOOL_MANAGED,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				D3DXCOLOR(0, 0, 0, 0),
				nullptr,
				nullptr,
				&texture);
		}

	} while (depth > 0);
}

void ASELoader::Read_GEOMOBJECT()
{
	GeomObj* geomObj = new GeomObj();
	UINT depth = 0;
	do
	{
		NextWord();
		if (m_word == L"{")	depth++;
		else if (m_word == L"}")	depth--;

		//else if (m_word == L"*NODE_NAME")		;
		else if (m_word == L"*NODE_TM")			Read_NODE_TM(geomObj->localMatrix, geomObj->localMatrixSR, geomObj->localMatrixT);
		else if (m_word == L"*MESH")			Read_MESH(geomObj);
		else if (m_word == L"*MATERIAL_REF")	geomObj->mtrlTex = &m_ase->m_mtrlTexList[NextInt()];
		 
	} while (depth > 0);
	m_ase->m_geomObj.push_back(geomObj);
}

void ASELoader::Read_NODE_TM(D3DXMATRIX& localMatrix, D3DXMATRIX& localMatrixSR, D3DXMATRIX& localMatrixT)
{
	ZeroMemory(&localMatrix, sizeof(D3DXMATRIX));

	UINT depth = 0;
	do
	{
		NextWord();
		if (m_word == L"{")	depth++;
		else if (m_word == L"}")	depth--;

		else if (m_word == L"*TM_ROW0")		{ localMatrix._11 = NextFloat();	localMatrix._13 = NextFloat();	localMatrix._12 = NextFloat();	localMatrix._14 = 0; }
		else if (m_word == L"*TM_ROW2")		{ localMatrix._21 = NextFloat();	localMatrix._23 = NextFloat();	localMatrix._22 = NextFloat();	localMatrix._24 = 0; }
		else if (m_word == L"*TM_ROW1")		{ localMatrix._31 = NextFloat();	localMatrix._33 = NextFloat();	localMatrix._32 = NextFloat();	localMatrix._34 = 0; }
		else if (m_word == L"*TM_ROW3")		{ localMatrix._41 = NextFloat();	localMatrix._43 = NextFloat();	localMatrix._42 = NextFloat();	localMatrix._44 = 1; }

	} while (depth > 0);

	ZeroMemory(&localMatrixSR, sizeof(D3DXMATRIX));
	ZeroMemory(&localMatrixT, sizeof(D3DXMATRIX));

	localMatrixSR = localMatrix;
	localMatrixSR._14 = localMatrixSR._24 = localMatrixSR._34 = localMatrixSR._41 = localMatrixSR._42 = localMatrixSR._43 = 0;
	localMatrixSR._44 = 1;
	D3DXMatrixTranslation(&localMatrixT, localMatrix._41, localMatrix._42, localMatrix._43);
}

void ASELoader::Read_MESH(GeomObj* geomObj)
{
	std::vector<Vertex_PNT>& vertexData = geomObj->vertexData;
	std::vector<D3DXVECTOR3> position;
	std::vector<D3DXVECTOR2> uv;

	UINT depth = 0;
	do
	{
		NextWord();
		if (m_word == L"{")	depth++;
		else if (m_word == L"}")	depth--;

		else if (m_word == L"*MESH_NUMVERTEX")		position.resize(NextInt());
		else if (m_word == L"*MESH_NUMFACES")		vertexData.resize(NextInt() * 3);
		else if (m_word == L"*MESH_NUMTVERTEX")		uv.resize(NextInt());

		else if (m_word == L"*MESH_VERTEX_LIST")	Read_MESH_VERTEX_LIST(position);
		else if (m_word == L"*MESH_FACE_LIST")		Read_MESH_FACE_LIST(vertexData, position);
		else if (m_word == L"*MESH_TVERTLIST")		Read_MESH_TVERTLIST(uv);
		else if (m_word == L"*MESH_TFACELIST")		Read_MESH_TFACELIST(vertexData, uv);
		else if (m_word == L"*MESH_NORMALS")		Read_MESH_NORMALS(vertexData);

	} while (depth > 0);

	D3DXMATRIX localMatrixInverse;
	D3DXMatrixInverse(&localMatrixInverse, nullptr, &(geomObj->localMatrixSR * geomObj->localMatrixT));
	for (auto& iter : vertexData)
	{
		D3DXVec3TransformCoord(&iter.position, &iter.position, &localMatrixInverse);
		D3DXVec3TransformNormal(&iter.normal, &iter.normal, &localMatrixInverse);
	}
}

void ASELoader::Read_MESH_VERTEX_LIST(std::vector<D3DXVECTOR3>& position)
{
	UINT depth = 0;
	do
	{
		NextWord();
		if (m_word == L"{")	depth++;
		else if (m_word == L"}")	depth--;

		else if (m_word == L"*MESH_VERTEX")
		{
			D3DXVECTOR3& place = position[NextInt()];
			place.x = NextFloat();
			place.z = NextFloat();
			place.y = NextFloat();
		}

	} while (depth > 0);
}

void ASELoader::Read_MESH_FACE_LIST(std::vector<Vertex_PNT>& vertexData, std::vector<D3DXVECTOR3>& position)
{
	UINT depth = 0;
	do
	{
		NextWord();
		if (m_word == L"{")	depth++;
		else if (m_word == L"}")	depth--;

		else if (m_word == L"*MESH_FACE")
		{
			UINT index = NextInt() * 3;
			NextWord();	vertexData[index + 0].position = position[NextInt()];
			NextWord();	vertexData[index + 2].position = position[NextInt()];
			NextWord();	vertexData[index + 1].position = position[NextInt()];
		}

	} while (depth > 0);
}

void ASELoader::Read_MESH_TVERTLIST(std::vector<D3DXVECTOR2>& uv)
{
	UINT depth = 0;
	do
	{
		NextWord();
		if (m_word == L"{")	depth++;
		else if (m_word == L"}")	depth--;

		else if (m_word == L"*MESH_TVERT")
		{
			D3DXVECTOR2& place = uv[NextInt()];
			place.x = NextFloat();
			place.y = 1.f - NextFloat();
		}

	} while (depth > 0);
}

void ASELoader::Read_MESH_TFACELIST(std::vector<Vertex_PNT>& vertexData, std::vector<D3DXVECTOR2>& uv)
{
	UINT depth = 0;
	do
	{
		NextWord();
		if (m_word == L"{")	depth++;
		else if (m_word == L"}")	depth--;

		else if (m_word == L"*MESH_TFACE")
		{
			UINT index = NextInt() * 3;
			vertexData[index + 0].uv = uv[NextInt()];
			vertexData[index + 2].uv = uv[NextInt()];
			vertexData[index + 1].uv = uv[NextInt()];
		}

	} while (depth > 0);
}

void ASELoader::Read_MESH_NORMALS(std::vector<Vertex_PNT>& vertexData)
{
	UINT index = 0;

	UINT depth = 0;
	do
	{
		NextWord();
		if (m_word == L"{")	depth++;
		else if (m_word == L"}")	depth--;

		else if (m_word == L"*MESH_FACENORMAL")	index = NextInt() * 3;
		else if (m_word == L"*MESH_VERTEXNORMAL")
		{
			NextWord();
			vertexData[index].normal.x = NextFloat();
			vertexData[index].normal.z = NextFloat();
			vertexData[index].normal.y = NextFloat();
			index++;
		}

	} while (depth > 0);
}










ASE::ASE()
{
}

ASE::~ASE()
{
	for (auto& iter : m_geomObj)
		SAFE_DELETE(iter);
	m_geomObj.clear();

	for (auto& iter : m_mtrlTexList)
		SAFE_RELEASE(iter.second);
	m_mtrlTexList.clear();
}



UINT ASE::GetAttributeCount()	{ return m_geomObj.size(); }

GeomObj*	ASE::GetGeomObj(const UINT& attribute)		{ return m_geomObj[attribute]; }
//MtrlTex *	ASE::GetMtrlTex(const UINT & attribute)		{ return m_geomObj[attribute]->mtrlTex; }
//D3DXMATRIX	ASE::GetMatrixSR(const UINT & attribute)	{ return m_geomObj[attribute]->localMatrixSR; }
//D3DXMATRIX	ASE::GetMatrixT(const UINT & attribute)		{ return m_geomObj[attribute]->localMatrixT; }
void		ASE::SetFVF(LPDIRECT3DDEVICE9 device)	{ device->SetFVF(Vertex_PNT::FVF); }

void ASE::Render(LPDIRECT3DDEVICE9 device, const UINT & attribute)
{
	GeomObj* geomObj = m_geomObj[attribute];
	device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, geomObj->vertexData.size() / 3, &geomObj->vertexData[0], sizeof(Vertex_PNT));
}
