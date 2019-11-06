#include "DXUT.h"
#include "TextureManager.h"



TextureManager::TextureManager()
{
}
TextureManager::~TextureManager()
{
	for (auto& tex : m_texList)
	{
		SAFE_RELEASE(tex.second->tex);
		SAFE_DELETE(tex.second);
	}
	m_texList.clear();
}



Texture * TextureManager::LoadTexture(const std::wstring & path, const std::wstring & key)
{
	{
		auto& findTexIter = m_texList.find(key);
		if (findTexIter != m_texList.end())
			return findTexIter->second;
	}

	Texture* tex = new Texture();
	if (tex)
	{
		if(SUCCEEDED( D3DXCreateTextureFromFileExW(DEVICE, path.data(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, NULL, &tex->info, nullptr, &tex->tex) ))
		{
			m_texList.insert(std::make_pair( key, tex ));
			return tex;
		}
		delete tex;
	}
	return nullptr;
}

Texture * TextureManager::GetTexture(const std::wstring & key)
{
	auto& findTexIter = m_texList.find(key);
	if (findTexIter != m_texList.end())
		return findTexIter->second;
	return nullptr;
}