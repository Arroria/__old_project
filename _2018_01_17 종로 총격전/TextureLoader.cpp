#include "stdafx.h"
#include "TextureLoader.h"


TextureLoader::TextureLoader()
{
}
TextureLoader::~TextureLoader()
{
}



void TextureLoader::RegisterTexture(LPDIRECT3DDEVICE9 device, const std::wstring & texKey, const std::wstring & texPath)
{
	auto& iter = m_textureContainer.find(texKey);
	if (iter == m_textureContainer.end())
	{
		for (auto& iter : m_textureContainer)
		{
			if (iter.second->path == texPath)
			{
				std::cout << "�ؽ��� �ε� �ߺ�\n\tPath : ";	std::wcout << texPath << std::endl;
				return;
			}
		}

		Texture* texture = new Texture;
		if (FAILED( D3DXCreateTextureFromFileExW(device, texPath.data(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DXCOLOR(0, 0, 0, 0), &texture->imageInfo, nullptr, &texture->texture) ))
		{
			std::cout << "�ؽ���%20�ε�%20����\n\tPath : ";	std::wcout << texPath;	std::cout << "\n\tKey : ";	std::wcout << texKey << std::endl;
			SAFE_DELETE(texture);
			return;
		}

		texture->path = texPath;
		m_textureContainer.insert(std::make_pair(texKey, texture));
		return;
	}
	std::cout << "�ؽ��� �ε� �ߺ�\n\tKey : ";	std::wcout << texKey << std::endl;
}

Texture * TextureLoader::GetTexture(const std::wstring & texKey) const
{
	auto& iter = m_textureContainer.find(texKey);
	if (iter == m_textureContainer.end())
	{
		std::cout << "�ؽ��� ȣ�� ���� : "; std::wcout << texKey << std::endl;
		return nullptr;
	}
	return iter->second;
}

void TextureLoader::ReleaseAllTexture()
{
	for (auto& iter : m_textureContainer)
	{
		SAFE_RELEASE(iter.second->texture);
		SAFE_DELETE(iter.second);
	}
	m_textureContainer.clear();
}
