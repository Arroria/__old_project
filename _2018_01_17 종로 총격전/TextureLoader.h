#pragma once
#include "Singleton.h"

class TextureLoader;
class Texture
{
private:
	friend TextureLoader;
	LPDIRECT3DTEXTURE9	texture;
	D3DXIMAGE_INFO		imageInfo;
	std::wstring		path;

public:
	LPDIRECT3DTEXTURE9 GetTexture() { return texture; }
	const D3DXIMAGE_INFO& GetImageInfo() const { return imageInfo; }
};

class TextureLoader :
	public Singleton<TextureLoader>
{
private:
	std::map<std::wstring, Texture*> m_textureContainer;

public:
	void RegisterTexture(LPDIRECT3DDEVICE9 device, const std::wstring& texKey, const std::wstring& texPath);
	Texture* GetTexture(const std::wstring& texKey) const;
	void ReleaseAllTexture();

public:
	TextureLoader();
	~TextureLoader();
};

