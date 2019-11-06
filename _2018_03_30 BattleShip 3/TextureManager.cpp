#include "DXUT.h"
#include "TextureManager.h"


TextureManager::TextureManager()
{
}
TextureManager::~TextureManager()
{
	for (auto& texture : m_texList)
	{
		SAFE_RELEASE(texture.second->tex);
		SAFE_DELETE(texture.second);
	}
	m_texList.clear();
}



Texture * TextureManager::LoadTexture(const std::wstring & path, const std::wstring & key)
{
	auto& texIter = m_texList.find(key);
	if (texIter != m_texList.end())
		return texIter->second;

	Texture* tex = new Texture;
	D3DXCreateTextureFromFileExW(DEVICE, path.data(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, NULL, &tex->info, nullptr, &tex->tex);
	assert(tex->tex);

	m_texList.insert(std::make_pair( key, tex ));
	return tex;
}

Texture * TextureManager::GetTexture(const std::wstring & key)
{
	auto& texIter = m_texList.find(key);
	if (texIter != m_texList.end())
		return texIter->second;
	return nullptr;
}
