#include "DXUT.h"
#include "TextureManager.h"


TextureManager::TextureManager()
{
}
TextureManager::~TextureManager()
{
	for (auto& tex : m_texList)
		SAFE_DELETE(tex.second);
	m_texList.clear();
}



Texture * TextureManager::LoadTex(const std::wstring & path, const std::wstring & key)
{
	/* Check */ {
		auto& findTex = m_texList.find(key);
		if (findTex != m_texList.end())
			return findTex->second;
	}

	Texture* tex = new Texture();
	if (tex)
	{
		if (FAILED( D3DXCreateTextureFromFileExW(DEVICE, path.data(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, NULL, &tex->info, nullptr, &tex->tex) ))
		{
			delete tex;
			return nullptr;
		}
	}

	m_texList.insert(std::make_pair( key, tex ));
	return tex;
}

Texture * TextureManager::GetTex(const std::wstring & key)
{
	auto& findTex = m_texList.find(key);
	if (findTex == m_texList.end())
		return nullptr;
	return findTex->second;
}
