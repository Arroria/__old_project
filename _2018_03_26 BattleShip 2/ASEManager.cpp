#include "DXUT.h"
#include "ASEManager.h"


ASEManager::ASEManager()
{
}
ASEManager::~ASEManager()
{
	for (auto& ase : m_aseList)
	{
		SAFE_RELEASE( ase.second->m_tex );
		SAFE_DELETE( ase.second );
	}
	m_aseList.clear();
}



ASE * ASEManager::LoadASE(const std::wstring & path, const std::wstring & name, const std::wstring & key)
{
	/* Find ASE Iter*/ {
		auto& aseIter = m_aseList.find(key);
		if (aseIter != m_aseList.end())
			return aseIter->second;
	}

	FILE* aseFile = nullptr;
	aseFile = _wfopen((path + L'/' + name).data(), L"r");
	if (!aseFile)
		return nullptr;

	std::wstring token;
	auto NextToken = [&]()->std::wstring&
	{
		token.clear();
		bool isString = false;
		while (!feof(aseFile))
		{
			wchar_t data = getwc(aseFile);
			if (data == '\"')
			{
				if (isString)
					break;
				isString = true;
				continue;
			}
			if (data < 33 &&!isString)
			{
				if (token.empty())
					continue;
				break;
			}

			token += data;
		}
		return token;
	};
	auto NextInt = [&]()->int
	{
		return _wtoi(NextToken().data());
	};
	auto NextFloat = [&]()->float
	{
		return (float)_wtof(NextToken().data());
	};

	ASE* ase = new ASE();

	do
	{
		NextToken();

		if (token == L"*MATERIAL_LIST")
			 {
				 D3DMATERIAL9& mtrl = ase->m_mtrl;

				 UINT depthlevel = 0;
				 do
				 {
					 NextToken();
						 if (token == L"{")	depthlevel++;
					else if (token == L"}")	depthlevel--;

					else if (token == L"*MATERIAL_AMBIENT")		{ mtrl.Ambient.r  = NextFloat();	mtrl.Ambient.g  = NextFloat();	mtrl.Ambient.b  = NextFloat();	mtrl.Ambient.a  = 1; }
					else if (token == L"*MATERIAL_DIFFUSE")		{ mtrl.Diffuse.r  = NextFloat();	mtrl.Diffuse.g  = NextFloat();	mtrl.Diffuse.b  = NextFloat();	mtrl.Diffuse.a  = 1; }
					else if (token == L"*MATERIAL_SPECULAR")	{ mtrl.Specular.r = NextFloat();	mtrl.Specular.g = NextFloat();	mtrl.Specular.b = NextFloat();	mtrl.Specular.a = 1; }
					else if (token == L"*BITMAP")				{
						LPDIRECT3DTEXTURE9 tex = nullptr;
						std::wstring dddd = path + L'/' + NextToken();
						D3DXCreateTextureFromFileExW(DEVICE, (dddd).data(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, NULL, nullptr, nullptr, &tex);
						ase->m_tex = tex;
					}
				 } while (depthlevel);
			 }
		if (token == L"*MESH")
		{
			 std::vector<D3DXVECTOR3> p;
			 std::vector<D3DXVECTOR2> t;

			int depthLevel = 0;
			do
			{
				NextToken();
					 if (token == L"{")	depthLevel++;
				else if (token == L"}")	depthLevel--;

				else if (token == L"*MESH_NUMVERTEX")	p.resize(NextInt());
				else if (token == L"*MESH_NUMFACES")	ase->m_vData.resize(NextInt() * 3);
				else if (token == L"*MESH_NUMTVERTEX")	t.resize(NextInt());
				 
				else if (token == L"*MESH_VERTEX_LIST")
					{
						int depthLevel = 0;
						do
						{
							NextToken();
								 if (token == L"{")	depthLevel++;
							else if (token == L"}")	depthLevel--;

							else if (token == L"*MESH_VERTEX")
							{
								D3DXVECTOR3& pos = p[NextInt()];
								pos.x = NextFloat();
								pos.z = NextFloat();
								pos.y = NextFloat();
							}
						} while (depthLevel);
					}
				else if (token == L"*MESH_FACE_LIST")
					{
						int depthLevel = 0;
						do
						{
							NextToken();
								 if (token == L"{")	depthLevel++;
							else if (token == L"}")	depthLevel--;

							else if (token == L"*MESH_FACE")
							{
								UINT index = NextInt() * 3;
								NextToken();	 ase->m_vData[index + 0].p = p[NextInt()];
								NextToken();	 ase->m_vData[index + 2].p = p[NextInt()];
								NextToken();	 ase->m_vData[index + 1].p = p[NextInt()];
							}
						} while (depthLevel);
					}
				else if (token == L"*MESH_TVERTLIST")
					{
						 int depthLevel = 0;
						 do
						 {
							 NextToken();
								  if (token == L"{")	depthLevel++;
							 else if (token == L"}")	depthLevel--;

							 else if (token == L"*MESH_TVERT")
							 {
								 D3DXVECTOR2& uv = t[NextInt()];
								 uv.x = NextFloat();
								 uv.y = 1 - NextFloat();
							 }
						 } while (depthLevel);
					}
				else if (token == L"*MESH_TFACELIST")
				{
					int depthLevel = 0;
					do
					{
						NextToken();
							if (token == L"{")	depthLevel++;
						else if (token == L"}")	depthLevel--;

						else if (token == L"*MESH_TFACE")
						 {
							UINT index = NextInt() * 3;
							ase->m_vData[index + 0].t = t[NextInt()];
							ase->m_vData[index + 2].t = t[NextInt()];
							ase->m_vData[index + 1].t = t[NextInt()];
						}
					} while (depthLevel);
				}
				else if (token == L"*MESH_NORMALS")
				{
					UINT index = 0;
					int depthLevel = 0;
					do
					{
						NextToken();
							  if (token == L"{")	depthLevel++;
						else if (token == L"}")	depthLevel--;

						else if (token == L"*MESH_FACENORMAL") { index = NextInt() * 3;}
						else if (token == L"*MESH_VERTEXNORMAL")
						{
							NextToken();
							D3DXVECTOR3& normal = ase->m_vData[index++].n;
							normal.x = NextFloat();
							normal.z = NextFloat();
							normal.y = NextFloat();
						}
					} while (depthLevel);
				}
			} while (depthLevel);
		}
	} while (!token.empty());

	fclose(aseFile);

	m_aseList.insert(std::make_pair( key, ase ));
	return ase;
}

ASE * ASEManager::GetASE(const std::wstring & key)
{
	auto& aseIter = m_aseList.find(key);
	return aseIter == m_aseList.end() ?
		nullptr :
		aseIter->second;
}





ASE::ASE()
{
	ZeroMemory(this, sizeof(ASE));
}
ASE::~ASE()
{
}



void ASE::Render()
{
	DEVICE->SetFVF(ASE::ASEVertex::FVF);
	DEVICE->SetTexture(0, m_tex);
	DEVICE->SetMaterial(&m_mtrl);
	DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vData.size() / 3, &m_vData[0], sizeof(ASEVertex));
}

