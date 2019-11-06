#include "MainHeader.h"
#include "TextureManager.h"


namespace AF
{
	TextureManager::TextureManager()
		: m_textureRenderer(nullptr)
	{
	}
	TextureManager::~TextureManager()
	{
		for (auto& iter : m_textureContainer)
		{
			SAFE_RELEASE(iter.second->sprite);
			SAFE_DELETE(iter.second);
		}
		m_textureContainer.clear();
		SAFE_RELEASE(m_textureRenderer);
	}



	HRESULT TextureManager::Initialize()
	{
		IF_FAILED_RETURN( D3DXCreateSprite(g_processManager->GetDevice(), &m_textureRenderer) );
		return S_OK;
	}



	HRESULT TextureManager::LoadTexture(const std::string & textureKey, const std::wstring & textureAddress)
	{
		AF::Texture* texture = new AF::Texture();
		if (SUCCEEDED(D3DXCreateTextureFromFileExW(g_processManager->GetDevice(), textureAddress.c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, &texture->imageInfo, NULL, &texture->sprite)))
		{
			m_textureContainer.insert(std::make_pair(textureKey, texture));
			return S_OK;
		}

		SAFE_DELETE(texture);
		return E_FAIL;
	}

	AF::Texture * TextureManager::GetTexture(const std::string & textureKey)
	{
		auto& iter = m_textureContainer.find(textureKey);
		if (iter == m_textureContainer.end())
			return nullptr;
		return iter->second;
	}

}