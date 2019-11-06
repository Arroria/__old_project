#include "stdafx.h"
#include "TextureLoader.h"


TextureLoader::TextureLoader()
{
}
TextureLoader::~TextureLoader()
{
	for (auto& iter : m_textureContainer)
		SAFE_DELETE(iter.second);
	m_textureContainer.clear();
}



Texture * TextureLoader::LoadTexture(const std::wstring & filePath, const std::wstring & key)
{
	auto& iter = m_textureContainer.find(key);
	if (iter != m_textureContainer.end())
		return iter->second;
	for (auto& iter : m_textureContainer)
	{
		if (iter.second->filePath == filePath)
			return iter.second;
	}


	Texture* texture = new Texture();
	D3DXCreateTextureFromFileExW(
		DEVICE,
		filePath.c_str(),
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		1,
		NULL,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DXCOLOR(0, 0, 0, 0),
		&texture->imageInfo,
		nullptr,
		&texture->texture);

	if (!texture->texture)
	{
		cout << "텍스쳐 경로 잘못적음 ㅇㅇ" << endl;
		return nullptr;
	}

	texture->filePath = filePath;
	m_textureContainer.insert(std::make_pair(key, texture));
	return texture;
}

Texture * TextureLoader::GetTexture(const std::wstring & key)
{
	auto texture = m_textureContainer.find(key);
	if (texture == m_textureContainer.end())
	{
		cout << "텍스쳐 키 잘못적음 ㅇㅇ" << endl;
		return nullptr;
	}
	return texture->second;
}
