#pragma once
#include "Singleton.h"

struct Texture
{
	LPDIRECT3DTEXTURE9 d3dxTexture;
	D3DXIMAGE_INFO imageInfo;
	
};

class TextureContainer
{
private:
	std::map<std::wstring, Texture*> m_textureStorage;

public:
	bool LoadTexture(const std::wstring& key, const std::wstring& path);

	Texture* GetTexture(const std::wstring& key);

public:
	TextureContainer();
	~TextureContainer();
};

class TextureRenderer
	: public Singleton<TextureRenderer>
{
private:
	LPD3DXSPRITE m_d3dxSpriteRenderer;

public:
	bool Begin();

	void SetTransform(const D3DXMATRIX& transformMatrix);
	void SetTransform(const D3DXVECTOR3& position, const D3DXVECTOR3& scale = D3DXVECTOR3(1, 1, 1));
	void SetTransform(const D3DXVECTOR3& position, const float& rotation, const D3DXVECTOR3& scale = D3DXVECTOR3(1, 1, 1));

	bool DrawTexture(const Texture* texture, const D3DXCOLOR& color = D3DXCOLOR(1, 1, 1, 1));

	void End();
public:
	TextureRenderer();
	~TextureRenderer();
};
