#pragma once
#include "Singleton.h"

struct Texture
{
public:
	LPDIRECT3DTEXTURE9 tex;
	D3DXIMAGE_INFO info;

public:
	Texture() { ZeroMemory(this, sizeof(Texture)); }
	~Texture() { if (tex) { tex->Release(); tex = nullptr; } }
};

class TextureManager :
	public Singleton<TextureManager>
{
private:
	std::map<std::wstring, Texture*> m_texList;

public:
	Texture* LoadTex(const std::wstring& path, const std::wstring& key);
	Texture* GetTex(const std::wstring& key);

public:
	TextureManager();
	~TextureManager();
};

#define TextureLoad(path, key)	(SingletonInstance(TextureManager)->LoadTex(path, key))
#define TextureGet(key)			(SingletonInstance(TextureManager)->GetTex(key))

