#pragma once

class PlayerCursor
{
private:
	Texture* m_texDefault;
	Texture* m_texTargetDetected;
	Texture* m_texPlayerAttackEffect;
	
	Texture* m_texKill;
	std::array<Texture*, 10> m_texKillNum;


	std::vector<UINT> m_playerAttackEffect;
	std::vector<UINT> m_playerKillEffect;
	
	UINT m_killStreak;
	UINT m_killView;

public:
	void PlayerAttackHitToEnemy()	{ m_playerAttackEffect.push_back(0); }
	void PlayerAttackKillToEnemy();

public:
	void Render(LPD3DXSPRITE sprite, const bool& targetDetected);

public:
	PlayerCursor();
	~PlayerCursor();
};

