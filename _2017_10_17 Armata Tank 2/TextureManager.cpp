#include "stdafx.h"
#include "TextureManager.h"

#define g_pDevice (g_processManager->GetDevice())

TextureContainer::TextureContainer()
{
}
TextureContainer::~TextureContainer()
{
	for (auto &iter : m_textureStorage)
	{
		if (iter.second)
		{
			SAFE_RELEASE( iter.second->d3dxTexture );
			delete iter.second;
		}
	}
	m_textureStorage.clear();
}


bool TextureContainer::LoadTexture(const std::wstring & key, const std::wstring & path)
{
	if (m_textureStorage.find(key) != m_textureStorage.end())
		return true;

	Texture* texture = new Texture();
	if (!texture)
		return true;

	if (SUCCEEDED( D3DXCreateTextureFromFileExW(g_pDevice, 
		path.c_str(), 
		D3DX_DEFAULT_NONPOW2, 
		D3DX_DEFAULT_NONPOW2, 
		0, 
		0, 
		D3DFMT_UNKNOWN, 
		D3DPOOL_MANAGED, 
		D3DX_DEFAULT, 
		D3DX_DEFAULT, 
		NULL, 
		&texture->imageInfo, 
		nullptr, 
		&texture->d3dxTexture) ))
	{
		m_textureStorage.insert(std::make_pair(key, texture));
		return false;
	}

	delete texture;
	return false;
}

Texture * TextureContainer::GetTexture(const std::wstring & key)
{
	return m_textureStorage.find(key)->second;
}




TextureRenderer::TextureRenderer()
	: m_d3dxSpriteRenderer(nullptr)
{
	D3DXCreateSprite(g_pDevice, &m_d3dxSpriteRenderer);
}
TextureRenderer::~TextureRenderer()
{
	SAFE_RELEASE(m_d3dxSpriteRenderer);
}


bool TextureRenderer::Begin()
{
	return FAILED( m_d3dxSpriteRenderer->Begin(D3DXSPRITE_ALPHABLEND) );
}

void TextureRenderer::SetTransform(const D3DXMATRIX & transformMatrix)
{
	m_d3dxSpriteRenderer->SetTransform(&transformMatrix);
}

void TextureRenderer::SetTransform(const D3DXVECTOR3 & position, const D3DXVECTOR3 & scale)
{
	D3DXMATRIX matrixS, matrixT;
	D3DXMatrixTranslation(&matrixT, position.x, position.y, 0);
	D3DXMatrixScaling(&matrixS, scale.x, scale.y, scale.z);
	SetTransform(matrixS * matrixT);
}

void TextureRenderer::SetTransform(const D3DXVECTOR3 & position, const float & rotation, const D3DXVECTOR3 & scale)
{
	D3DXMATRIX matrixS, matrixR, matrixT;
	D3DXMatrixTranslation(&matrixT, position.x, position.y, 0);
	D3DXMatrixRotationZ(&matrixR, D3DXToRadian(rotation));
	D3DXMatrixScaling(&matrixS, scale.x, scale.y, scale.z);
	SetTransform(matrixS * matrixR * matrixT);
}

bool TextureRenderer::DrawTexture(const Texture * texture, const D3DXCOLOR & color)
{
	if (!texture || !texture->d3dxTexture)
		return true;
	return FAILED( m_d3dxSpriteRenderer->Draw(texture->d3dxTexture, nullptr, &D3DXVECTOR3(texture->imageInfo.Width * 0.5f, texture->imageInfo.Height * 0.5f, 0), nullptr, color) );
}

void TextureRenderer::End()
{
	m_d3dxSpriteRenderer->End();
}

