#pragma once
#include "Singleton.h"

class TempShaderHelper
	: public Singleton<TempShaderHelper>
{
private:
	std::map<std::wstring, LPD3DXEFFECT> m_shaderContainer;
	LPD3DXEFFECT m_shader;

public:
	LPD3DXEFFECT LoadShader(LPDIRECT3DDEVICE9 device, const std::wstring& path, const std::wstring& key);
	void SeleteShader(const std::wstring& key);

	void SetTexture(const std::string& value, LPDIRECT3DTEXTURE9 texture);
	void SetMaterial(const std::string& value, const D3DMATERIAL9& mtrl);
	void SetValue(const std::string & value, const void* dataPtr, const UINT& dataSize);

	UINT Begin(LPDIRECT3DDEVICE9 device);
	void BeginDraw(const UINT& pass);
	void EndDraw();
	void End();

public:
	TempShaderHelper();
	~TempShaderHelper();
};

