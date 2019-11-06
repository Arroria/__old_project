#pragma once
class GameUI
{
private:
	std::pair<UINT, UINT> m_frame;
	bool m_isLeft;

private:
	std::wstring P_E_Check() { return m_isLeft ? L"p" : L"e"; }

public:
	void Update();
	void RenderBegin();
	void RenderBase	(const D3DXVECTOR2& pos);
	void RenderShip	(const D3DXVECTOR2& pos, const UINT& index, const bool& isExpired, const bool& isDark = false);
	void RenderItem	(const D3DXVECTOR2& pos, const UINT& index, const bool& isGlow);
	void RenderBtn	(const D3DXVECTOR2& pos, const UINT& index, const bool& isGlow);
	void RenderBtnInfo(const UINT& index);
	void RenderEnd() { SPRITE->End(); }

public:
	GameUI(const bool & isLeft);
	~GameUI();
};

