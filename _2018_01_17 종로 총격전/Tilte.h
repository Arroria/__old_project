#pragma once
#include "SceneManager.h"
class Tilte :
	public Scene
{
private:
	std::array<UINT, 4> m_btState;
	std::wstring m_overScene;

public:
	void Initialize() override;
	void Update() override;
	void Render() override;
	void Release() override;

public:
	Tilte();
	~Tilte();
};

