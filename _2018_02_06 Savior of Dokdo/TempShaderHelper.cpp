#include "stdafx.h"
#include "TempShaderHelper.h"


TempShaderHelper::TempShaderHelper()
	: m_shader(nullptr)
{
}
TempShaderHelper::~TempShaderHelper()
{
	for (auto& shader : m_shaderContainer)
		SAFE_RELEASE(shader.second);
	m_shaderContainer.clear();
}



LPD3DXEFFECT TempShaderHelper::LoadShader(LPDIRECT3DDEVICE9 device, const std::wstring & path, const std::wstring & key)
{
	LPD3DXEFFECT shader = nullptr;
	LPD3DXBUFFER errorBuffer = nullptr;
	DWORD flag = NULL;
#if _DEBUG
	flag = D3DXSHADER_DEBUG;
#endif
	D3DXCreateEffectFromFileExW(device, path.c_str(), nullptr, nullptr, nullptr, flag, NULL, &shader, &errorBuffer);

	if (errorBuffer)
	{
		if (!shader)
			MessageBoxA(g_processManager->GetWndInfo()->hWnd, (LPCSTR)errorBuffer->GetBufferPointer(), nullptr, NULL);
		errorBuffer->Release();
	}


	if (shader)
		m_shaderContainer.insert(std::make_pair(key, shader));
	return shader;
}

void TempShaderHelper::SeleteShader(const std::wstring & key)
{
	auto shader = m_shaderContainer.find(key);
	m_shader = shader == m_shaderContainer.end() ?
		nullptr :
		shader->second;
}

void TempShaderHelper::SetTexture(const std::string & value, LPDIRECT3DTEXTURE9 texture)
{
	m_shader->SetTexture(value.c_str(), texture);
}

void TempShaderHelper::SetMaterial(const std::string & value, const D3DMATERIAL9& mtrl)
{
	struct MtrlDAS
	{
		D3DCOLORVALUE diffuse;
		D3DCOLORVALUE ambient;
		D3DCOLORVALUE specular;
		MtrlDAS(const D3DCOLORVALUE& d, const D3DCOLORVALUE& a, const D3DCOLORVALUE& s) : diffuse(d), ambient(a), specular(s) {}
	};
	m_shader->SetValue(value.c_str(), (void*)(&MtrlDAS(mtrl.Diffuse, mtrl.Ambient, mtrl.Specular)), sizeof(MtrlDAS));
}

void TempShaderHelper::SetValue(const std::string & value, const void* dataPtr, const UINT& dataSize)
{
	m_shader->SetValue(value.data(), dataPtr, dataSize);
}


UINT TempShaderHelper::Begin(LPDIRECT3DDEVICE9 device)
{
	LPDIRECT3DBASETEXTURE9 texture = nullptr;
	m_shader->GetTexture("g_texture", &texture);
	m_shader->SetBool("g_isTexturing", texture ? true : false);

	m_shader->SetVector("g_lightPos", &D3DXVECTOR4(0, 1000, 0, 1));
	m_shader->SetVector("g_viewPos", &D3DXVECTOR4(GetSingleton(CameraManager)->GetData().position, 1));

	D3DXMATRIX wM, vM, pM;
	device->GetTransform(D3DTS_WORLD, &wM);
	device->GetTransform(D3DTS_VIEW, &vM);
	device->GetTransform(D3DTS_PROJECTION, &pM);

	m_shader->SetMatrix("g_wM", &wM);
	m_shader->SetMatrix("g_vM", &vM);
	m_shader->SetMatrix("g_pM", &pM);

	UINT passCount = 0;
	m_shader->Begin(&passCount, NULL);
	return passCount;
}

void TempShaderHelper::BeginDraw(const UINT & pass)
{
	m_shader->BeginPass(pass);
}

void TempShaderHelper::EndDraw()
{
	m_shader->EndPass();
}

void TempShaderHelper::End()
{
	m_shader->End();
}
