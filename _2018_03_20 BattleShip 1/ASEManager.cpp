#include "DXUT.h"
#include "ASEManager.h"


ASEManager::ASEManager()
	: m_file(nullptr)
{
}
ASEManager::~ASEManager()
{
	for (auto& iter : m_aseList)
	{
		SAFE_RELEASE( iter.second->m_tex );
		SAFE_DELETE( iter.second );
 	}
	m_aseList.clear();
}



std::wstring & ASEManager::NextToken()
{
	m_data.clear();

	bool isString = false;
	while (!feof(m_file))
	{
		wchar_t data = getwc(m_file);
		
		if (data == '\"')
		{
			if (isString)
				break;
			isString = true;
			continue;
		}

		if (data <= 32 && !isString)
		{
			if (m_data.empty())
				continue;
			break;
		}

		m_data += data;
	}

	return m_data;
}
int ASEManager::NextInt()		{ return _wtoi(NextToken().data()); }
float ASEManager::NextFloat()	{ return _wtof(NextToken().data()); }

EasyASE * ASEManager::LoadASE(const std::wstring & path, const std::wstring & name, const std::wstring & key)
{
	auto& iter = m_aseList.find(key);
	if (iter != m_aseList.end())
		return iter->second;

	m_file = nullptr;
	m_file = _wfopen((path + L"/" + name).data(), L"r");
	if (!m_file)
		return nullptr;

	m_path = path;
	m_name = name;
	m_ase = nullptr;
	m_ase = new EasyASE;

	while (!NextToken().empty())
	{
		if (m_data == L"*MATERIAL_LIST")	MTRLTEX();
		if (m_data == L"*GEOMOBJECT")		MESH();
	};
	fclose(m_file);

	m_aseList.insert(std::make_pair(key, m_ase));
	return m_ase;
}

EasyASE * ASEManager::GetASE(const std::wstring & key)
{
	auto& iter = m_aseList.find(key);
	if (iter == m_aseList.end())
		return nullptr;
	return iter->second;
}

void ASEManager::MTRLTEX()
{
	int level = 0;
	do
	{
		NextToken();
			 if (m_data == L"{")	level++;
		else if (m_data == L"}")	level--;

		else if (m_data == L"*MATERIAL_AMBIENT")	{ m_ase->m_mtrl.Ambient .r = NextFloat();	m_ase->m_mtrl.Ambient .g = NextFloat();	m_ase->m_mtrl.Ambient .b = NextFloat();	m_ase->m_mtrl.Ambient.a = 1; }
		else if (m_data == L"*MATERIAL_DIFFUSE")	{ m_ase->m_mtrl.Diffuse .r = NextFloat();	m_ase->m_mtrl.Diffuse .g = NextFloat();	m_ase->m_mtrl.Diffuse .b = NextFloat();	m_ase->m_mtrl.Diffuse.a = 1; }
		else if (m_data == L"*MATERIAL_SPECULAR")	{ m_ase->m_mtrl.Specular.r = NextFloat();	m_ase->m_mtrl.Specular.g = NextFloat();	m_ase->m_mtrl.Specular.b = NextFloat();	m_ase->m_mtrl.Specular.a = 1; }
		else if (m_data == L"*BITMAP")				D3DXCreateTextureFromFileExW(DEVICE, (m_path + L"/" + NextToken()).data(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, NULL, nullptr, nullptr, &m_ase->m_tex);
	} while (level);
}

void ASEManager::MESH()
{
	std::vector<D3DXVECTOR3> p;
	std::vector<D3DXVECTOR2> t;

	int level = 0;
	do
	{
		NextToken();

			 if (m_data == L"{")	level++;
		else if (m_data == L"}")	level--;

		else if (m_data == L"*MESH_NUMVERTEX")	p.resize(NextInt());
		else if (m_data == L"*MESH_NUMFACES")	m_ase->m_vertex.resize(NextInt() * 3);
		else if (m_data == L"*MESH_NUMTVERTEX")	t.resize(NextInt());

		else if (m_data == L"*MESH_VERTEX_LIST")
		{
			int _level = 0;
			do
			{
				NextToken();

					 if (m_data == L"{")	_level++;
				else if (m_data == L"}")	_level--;

				else if (m_data == L"*MESH_VERTEX")
				{
					D3DXVECTOR3& place = p[NextInt()];
					place.x = NextFloat();
					place.z = NextFloat();
					place.y = NextFloat();
				}
			} while (_level);
		}
		else if (m_data == L"*MESH_FACE_LIST")
		{
			int _level = 0;
			do
			{
				NextToken();

					 if (m_data == L"{")	_level++;
				else if (m_data == L"}")	_level--;

				else if (m_data == L"*MESH_FACE")
				{
					int index = NextInt() * 3;
					NextToken();	m_ase->m_vertex[index + 0].p = p[NextInt()];
					NextToken();	m_ase->m_vertex[index + 2].p = p[NextInt()];
					NextToken();	m_ase->m_vertex[index + 1].p = p[NextInt()];
				}
			} while (_level);
		}
		else if (m_data == L"*MESH_TVERTLIST")
		{
			int _level = 0;
			do
			{
				NextToken();

					 if (m_data == L"{")	_level++;
				else if (m_data == L"}")	_level--;

				else if (m_data == L"*MESH_TVERT")
				{
					D3DXVECTOR2& place = t[NextInt()];
					place.x = NextFloat();
					place.y = 1 - NextFloat();
				}
			} while (_level);
		}
		else if (m_data == L"*MESH_TFACELIST")
		{
			int _level = 0;
			do
			{
				NextToken();

				if (m_data == L"{")	_level++;
				else if (m_data == L"}")	_level--;

				else if (m_data == L"*MESH_TFACE")
				{
					int index = NextInt() * 3;
					m_ase->m_vertex[index + 0].t = t[NextInt()];
					m_ase->m_vertex[index + 2].t = t[NextInt()];
					m_ase->m_vertex[index + 1].t = t[NextInt()];
				}
			} while (_level);
		}
		else if (m_data == L"*MESH_NORMALS")
		{
			int index = 0;

			int _level = 0;
			do
			{
				NextToken();

				if (m_data == L"{")	_level++;
				else if (m_data == L"}")	_level--;

				else if (m_data == L"*MESH_FACENORMAL") index = NextInt() * 3;
				else if (m_data == L"*MESH_VERTEXNORMAL")
				{
					D3DXVECTOR3& place = m_ase->m_vertex[index + 0].n;
					NextToken();
					place.x = NextFloat();
					place.z = NextFloat();
					place.y = NextFloat();
					index++;
				}
			} while (_level);
		}

	} while (level);
}



void EasyASE::Render()
{
	DEVICE->SetFVF(ASEVertex::FVF);
	DEVICE->SetMaterial(&m_mtrl);
	DEVICE->SetTexture(0, m_tex);
	DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vertex.size() / 3, &m_vertex[0], sizeof(ASEVertex));
}
