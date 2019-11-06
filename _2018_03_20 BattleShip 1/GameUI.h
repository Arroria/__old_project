#pragma once

class Ship;
class GameUI
{
private:
	bool m_isRed;
	
	std::array<UINT, 5> m_shipState;
	std::array<bool, 4> m_skillState;

	UINT m_frame;

public:
	void ShipSetted(const UINT& index);
	void ShipDestroyed(const UINT& index);
	void SkillUsed(const UINT& index);

public:
	void Update();
	void Render(const std::array<Ship*, 5>& shipList, const D3DXVECTOR2& pos, const bool& isPlaying = true, const UINT& selectCursorPos = 0);

public:
	GameUI(const bool& isRed = false);
	~GameUI();
};

