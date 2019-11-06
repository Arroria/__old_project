#pragma once
namespace AF
{
	struct Texture
	{
		LPDIRECT3DTEXTURE9	sprite;
		D3DXIMAGE_INFO		imageInfo;
	};

	class TextureManager
	{
	private:
		LPD3DXSPRITE						m_textureRenderer;
		std::map<std::string, AF::Texture*>	m_textureContainer;



	public:
		HRESULT	LoadTexture	(const std::string& textureKey, const std::wstring& textureAddress);

	public:
		AF::Texture*	GetTexture	(const std::string& textureKey);



	public:
		HRESULT	Begin()																							{ return m_textureRenderer->Begin(D3DXSPRITE_ALPHABLEND); }
		HRESULT	SetTransform	(const D3DXMATRIX& mat)															{ return m_textureRenderer->SetTransform(&mat); }
		HRESULT	TextureRender	(const AF::Texture* texture, const RECT* ±ÍÂú¾ÆÀÌÁ¨ = nullptr, const D3DXCOLOR& color = D3DXCOLOR(1, 1, 1, 1))	{ return (texture ? m_textureRenderer->Draw(texture->sprite, ±ÍÂú¾ÆÀÌÁ¨, nullptr, nullptr, color) : E_FAIL); }
		HRESULT	End()																							{ return m_textureRenderer->End(); }



	public:
		HRESULT	Initialize();

	public:
		TextureManager();
		~TextureManager();
	};
}

