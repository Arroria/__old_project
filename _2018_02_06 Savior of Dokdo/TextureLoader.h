#pragma once
#include "Singleton.h"

struct Texture
{
	LPDIRECT3DTEXTURE9 texture;
	D3DXIMAGE_INFO imageInfo;
	std::wstring filePath;
	Texture() : texture(nullptr) { ZeroMemory(&imageInfo, sizeof(D3DXIMAGE_INFO)); }
	~Texture() { SAFE_RELEASE(texture); }
};

class TextureLoader :
	public Singleton<TextureLoader>
{
private:
	std::map<std::wstring, Texture*> m_textureContainer;

public:
	Texture* LoadTexture(const std::wstring& filePath, const std::wstring& key);
	Texture* GetTexture(const std::wstring& key);

public:
	TextureLoader();
	~TextureLoader();
};

