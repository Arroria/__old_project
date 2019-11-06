#pragma once
class GameUI
{
private:
	bool m_forPlayer;
	std::pair<UINT, UINT> m_frame;

private:
	void Render(const std::wstring& key, const D3DXVECTOR2& pos, const bool& darkness, const bool& isMiddel = false);
	std::wstring GetPE() { return m_forPlayer ? L"p" : L"e"; }

public:
	void Update();
	void R_Frame	(const D3DXVECTOR2& pos);
	void R_Item		(const D3DXVECTOR2& pos, const UINT& index, const bool& shiny);
	void R_Tool		(const D3DXVECTOR2& pos, const UINT& index, const bool& shiny);
	void R_ToolBox	(const UINT& index);
	void R_Ship		(const D3DXVECTOR2& pos, const UINT & size, const bool& shiny, const bool& noise);

public:
	GameUI(const bool& forPlayer);
	~GameUI();
};

