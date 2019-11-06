#pragma once
#include "SceneManager.h"
class Main :
	public BaseScene
{
private:
	std::map<std::wstring, LPDIRECT3DTEXTURE9> m_texture;

	std::array<UINT, 5> m_btState;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;

public:
	Main();
	~Main();
};

