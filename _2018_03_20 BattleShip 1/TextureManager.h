#pragma once
#include "Singleton.h"

struct Texture
{
	LPDIRECT3DTEXTURE9 tex;
	D3DXIMAGE_INFO info;
	Texture() { ZeroMemory(this, sizeof(Texture)); }
};

class TextureManager :
	public Singleton<TextureManager>
{
private:
	std::map<std::wstring, Texture*> m_textureList;

public:
	Texture* LoadTexture(const std::wstring& path, const std::wstring& key);
	Texture* GetTexture(const std::wstring& key);

public:
	TextureManager();
	~TextureManager();
};

#define LoadTex(path, key)	(SingletonInstance(TextureManager)->LoadTexture(path, key))
#define GetTex(key)	(SingletonInstance(TextureManager)->GetTexture(key))
