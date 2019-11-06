#include "DXUT.h"
#include "ASEManager.h"


ASEManager::ASEManager()
{
}
ASEManager::~ASEManager()
{
	for (auto& ase : m_aseList)
	{
		SAFE_RELEASE(ase.second->m_tex);
		SAFE_DELETE(ase.second);
	}
	m_aseList.clear();
}



ASE * ASEManager::LoadASE(const std::wstring & path, const std::wstring & name, const std::wstring & key)
{
	{
		auto& findASEIter = m_aseList.find(key);
		if (findASEIter != m_aseList.end())
			return findASEIter->second;
	}


	FILE* aseFile = _wfopen((path + L'/' + name).data(), L"r");
	if (!aseFile)
	{
		assert(aseFile);
		return nullptr;
	}
	ASE* ase = new ASE();


	std::wstring data;
	auto NextToken	= [&]()->std::wstring&
	{
		data.clear();
		bool isString = false;
		while (!feof(aseFile))
		{
			wchar_t wch = fgetwc(aseFile);

			if (wch == '\"')
			{
				if (isString)
					break;
				isString = true;
				continue;
			}

			if (wch < 33 && !isString)
			{
				if (data.size())
					break;
				continue;
			}

			data += wch;
		}
		return data;
	};
	auto NextInt	= [&]()->int	{ return _wtoi(NextToken().data()); };
	auto NextFloat	= [&]()->float	{ return (float)_wtof(NextToken().data()); };


	while (NextToken().size())
	{
		if (data == L"*SCENE");
		else if (data == L"*MATERIAL_LIST")
		{
			int level = 0;
			do
			{
				NextToken();

				if (data == L"{")		level++;
				else if (data == L"}")	level--;

				else if (data == L"*MATERIAL_AMBIENT")	{ auto& color = ase->m_mtrl.Ambient;	color.r = NextFloat();	color.g = NextFloat();	color.b = NextFloat();	color.a = 1; }
				else if (data == L"*MATERIAL_DIFFUSE")	{ auto& color = ase->m_mtrl.Diffuse;	color.r = NextFloat();	color.g = NextFloat();	color.b = NextFloat();	color.a = 1; }
				else if (data == L"*MATERIAL_SPECULAR")	{ auto& color = ase->m_mtrl.Specular;	color.r = NextFloat();	color.g = NextFloat();	color.b = NextFloat();	color.a = 1; }
				else if (data == L"*BITMAP")			D3DXCreateTextureFromFileExW(DEVICE, (path + L'/'+ NextToken()).data(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, NULL, nullptr, nullptr, &ase->m_tex);
			} while (level);
		}
		else if (data == L"*MESH")
		{
			std::vector<ASE::ASEVertex>& aseData = ase->m_vertexData;
			std::vector<D3DXVECTOR3> pData;
			std::vector<D3DXVECTOR2> tData;

			int level = 0;
			do
			{
				NextToken();

				if (data == L"{")		level++;
				else if (data == L"}")	level--;
				
				else if (data == L"*MESH_NUMVERTEX")	pData.resize(NextInt());
				else if (data == L"*MESH_NUMFACES")		aseData.resize(NextInt() * 3);
				else if (data == L"*MESH_NUMTVERTEX")	tData.resize(NextInt());

				else if (data == L"*MESH_VERTEX_LIST")
				{
					int level = 0;
					do
					{
						NextToken();
						if (data == L"{")		level++;
						else if (data == L"}")	level--;

						else if (data == L"*MESH_VERTEX")
						{
							D3DXVECTOR3& p = pData[NextInt()];
							p.x = NextFloat();
							p.z = NextFloat();
							p.y = NextFloat();
						}
					} while (level);
				}
				else if (data == L"*MESH_FACE_LIST")
				{
					int level = 0;
					do
					{
						NextToken();
						if (data == L"{")		level++;
						else if (data == L"}")	level--;

						else if (data == L"*MESH_FACE")
						{
							UINT index = NextInt() * 3;
							NextToken();	aseData[index + 0].p = pData[NextInt()];
							NextToken();	aseData[index + 2].p = pData[NextInt()];
							NextToken();	aseData[index + 1].p = pData[NextInt()];
						}
					} while (level);
				}
				else if (data == L"*MESH_TVERTLIST")
				{
					int level = 0;
					do
					{
						NextToken();
						if (data == L"{")		level++;
						else if (data == L"}")	level--;

						else if (data == L"*MESH_TVERT")
						{
							D3DXVECTOR2& t = tData[NextInt()];
							t.x = NextFloat();
							t.y = 1 - NextFloat();
						}
					} while (level);
				}
				else if (data == L"*MESH_TFACELIST")
				{
					int level = 0;
					do
					{
						NextToken();
						if (data == L"{")		level++;
						else if (data == L"}")	level--;

						else if (data == L"*MESH_TFACE")
						{
							UINT index = NextInt() * 3;
							aseData[index + 0].t = tData[NextInt()];
							aseData[index + 2].t = tData[NextInt()];
							aseData[index + 1].t = tData[NextInt()];
						}
					} while (level);
				}

				else if (data == L"*MESH_NORMALS")
				{
					UINT index = 0;
					UINT offset = 0;

					int level = 0;
					do
					{
						NextToken();
						if (data == L"{")		level++;
						else if (data == L"}")	level--;

						else if (data == L"*MESH_FACENORMAL")	{ index = NextInt(); offset = 0; }
						else if (data == L"*MESH_VERTEXNORMAL")
						{
							NextToken();
							aseData[index + offset].n.x = NextFloat();
							aseData[index + offset].n.z = NextFloat();
							aseData[index + offset].n.y = NextFloat();

							offset = (offset ? 1 : 2);
						}
					} while (level);
				}
			} while (level);
		}
	}


	fclose(aseFile);
	m_aseList.insert(std::make_pair( key, ase ));
	return ase;
}

ASE * ASEManager::GetASE(const std::wstring & key)
{
	auto& findASEIter = m_aseList.find(key);
	if (findASEIter != m_aseList.end())
		return findASEIter->second;
	return nullptr;
}






ASE::ASE()
	: m_tex	(nullptr)
	, m_mtrl(WHITEMTRL)
{
}
ASE::~ASE()
{
}



void ASE::Render()
{
	DEVICE->SetFVF(ASEVertex::FVF);
	DEVICE->SetTexture(0, m_tex);
	DEVICE->SetMaterial(&m_mtrl);
	DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vertexData.size() / 3, &m_vertexData[0], sizeof(ASEVertex));
}
