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
}



ASE * ASEManager::LoadASE(const std::wstring & path, const std::wstring & name, const std::wstring & key)
{
	auto& aseIter = m_aseList.find(key);
	if (aseIter != m_aseList.end())
		return aseIter->second;

	FILE* aseFile = _wfopen((path + L'/' + name).data(), L"r");
	if (!aseFile)
	{
		assert(aseFile);
		return nullptr;
	}

	std::wstring data;
	auto NextToken = [&data, &aseFile]()->std::wstring&
	{
		data.clear();

		bool isString = false;
		while (!feof(aseFile))
		{
			wchar_t wch = getwc(aseFile);

			if (wch == L'\"')
			{
				if (isString)
					break;
				isString = true;
				continue;
			}

			if (wch < 33)
			{
				if (!isString)
				{
					if (data.empty())
						continue;
					break;
				}
			}

			data += wch;
		}
		return data;
	};
	auto NextInt = [&NextToken]()->int
	{
		return _wtoi(NextToken().data());
	};
	auto NextFloat = [&NextToken]()->float
	{
		return (float)_wtof(NextToken().data());
	};
	
	ASE* ase = new ASE;
	while (NextToken().size())
	{
			 if (data == L"*SCENE")
			{
				int level = 0;
				do
				{
					NextToken();
						 if (data == L"{") level++;
					else if (data == L"}") level--;
				} while (level);
				int a = 5;
			}
		else if (data == L"*MATERIAL")
		{
			D3DMATERIAL9& mtrl = ase->m_mtrl;
			LPDIRECT3DTEXTURE9 tex = nullptr;

			NextToken();
			int level = 0;
			do
			{
				NextToken();

				if (data == L"{") level++;
				else if (data == L"}") level--;

				else if (data == L"AMBIENT")	{ mtrl.Ambient.r	= NextFloat();	mtrl.Ambient.g	= NextFloat();mtrl.Ambient.b	= NextFloat();mtrl.Ambient.a	= 1; }
				else if (data == L"DIFFUSE")	{ mtrl.Diffuse.r	= NextFloat();	mtrl.Diffuse.g	= NextFloat();mtrl.Diffuse.b	= NextFloat();mtrl.Diffuse.a	= 1; }
				else if (data == L"SPECULAR")	{ mtrl.Specular.r	= NextFloat();	mtrl.Specular.g	= NextFloat();mtrl.Specular.b	= NextFloat();mtrl.Specular.a	= 1; }
				else if (data == L"*BITMAP")	D3DXCreateTextureFromFileExW(DEVICE, (path + L'/' + NextToken()).data(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, NULL, nullptr, nullptr, &tex);

			} while (level);

			ase->m_tex = tex;
		}
		else if (data == L"*MESH")
		{
			std::vector<D3DXVECTOR3> p;
			std::vector<D3DXVECTOR2> t;
			std::vector<ASE::ASEVertex>& aseV = ase->m_vertex;

			int level = 0;
			do
			{
				NextToken();

				if (data == L"{") level++;
				else if (data == L"}") level--;

				else if (data == L"*MESH_NUMVERTEX")	p.resize(NextInt());
				else if (data == L"*MESH_NUMFACES")		aseV.resize(NextInt() * 3);
				else if (data == L"*MESH_NUMTVERTEX")	t.resize(NextInt());
				else if (data == L"*MESH_VERTEX_LIST")
				{
					int level = 0;
					do
					{
						NextToken();

						if (data == L"{") level++;
						else if (data == L"}") level--;

						else if (data == L"*MESH_VERTEX")
						{
							D3DXVECTOR3& vec = p[NextInt()];
							vec.x = NextFloat();
							vec.z = NextFloat();
							vec.y = NextFloat();
						}
					} while (level);
				}
				else if (data == L"*MESH_FACE_LIST")
				{
					int level = 0;
					do
					{
						NextToken();

						if (data == L"{") level++;
						else if (data == L"}") level--;

						else if (data == L"*MESH_FACE")
						{
							UINT index = NextInt() * 3;
							NextToken();	aseV[index + 0].p = p[NextInt()];
							NextToken();	aseV[index + 2].p = p[NextInt()];
							NextToken();	aseV[index + 1].p = p[NextInt()];
						}

					} while (level);
				}
				else if (data == L"*MESH_TVERTLIST")
				{
					int level = 0;
					do
					{
						NextToken();

						if (data == L"{") level++;
						else if (data == L"}") level--;

						else if (data == L"*MESH_TVERT")
						{
							D3DXVECTOR2& tVer = t[NextInt()];
							tVer.x = NextFloat();
							tVer.y = 1 - NextFloat();
						}

					} while (level);
				}
				else if (data == L"*MESH_TFACELIST")
				{
					int level = 0;
					do
					{
						NextToken();

						if (data == L"{") level++;
						else if (data == L"}") level--;

						else if (data == L"*MESH_TFACE")
						{
							UINT index = NextInt() * 3;
							aseV[index + 0].t = t[NextInt()];
							aseV[index + 2].t = t[NextInt()];
							aseV[index + 1].t = t[NextInt()];
						}

					} while (level);
				}
				else if (data == L"*MESH_NORMALS")
				{
					UINT index = 0;
					UINT count = 0;
					int level = 0;
					do
					{
						NextToken();

						if (data == L"{") level++;
						else if (data == L"}") level--;

						else if (data == L"*MESH_FACENORMAL")
						{
							index = NextInt() * 3;
							count = 0;
						}
						else if (data == L"*MESH_VERTEXNORMAL")
						{
							D3DXVECTOR3& nor = aseV[index + count].n;
							nor.x = NextFloat();
							nor.z = NextFloat();
							nor.y = NextFloat();

							if (!count)
								count = 2;
							else
								count = 1;
						}

					} while (level);
				}

			} while (level);
		}
	}
	fclose(aseFile);

	m_aseList.insert(std::make_pair(key, ase));
	return ase;
}

ASE * ASEManager::GetASE(const std::wstring & key)
{
	auto& aseIter = m_aseList.find(key);
	if (aseIter != m_aseList.end())
		return aseIter->second;
	return nullptr;
}





void ASE::Render()
{
	DEVICE->SetTexture(0, m_tex);
	DEVICE->SetMaterial(&m_mtrl);
	DEVICE->SetFVF(ASEVertex::FVF);
	DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vertex.size() / 3, &m_vertex[0], sizeof(ASEVertex));
}
