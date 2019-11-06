#include "DXUT.h"
#include "TextureManager.h"


TextureManager::TextureManager()
{
}
TextureManager::~TextureManager()
{
	for (auto& iter : m_textureList)
	{
		SAFE_RELEASE( iter.second->tex );
		SAFE_DELETE( iter.second );
	}
	m_textureList.clear();
}



Texture * TextureManager::LoadTexture(const std::wstring & path, const std::wstring & key)
{
	auto& iter = m_textureList.find(key);
	if (iter != m_textureList.end())
		return (*iter).second;

	Texture* texture = new Texture();
	if (FAILED( D3DXCreateTextureFromFileExW(DEVICE, path.data(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, NULL, &texture->info, nullptr, &texture->tex) ))
	{
		delete texture;
		return nullptr;
	}

	m_textureList.insert(std::make_pair( key, texture ));
	return texture;
}

Texture * TextureManager::GetTexture(const std::wstring & key)
{
	auto& iter = m_textureList.find(key);
	if (iter == m_textureList.end())
		return nullptr;

	return iter->second;
}
