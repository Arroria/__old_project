#pragma once

class Enemy;

struct LockonedTarget
{
	Enemy*		enemy;
	D3DXVECTOR2	wndPos;
	float		distance;
	UINT		time;
};

class Lockoner
{
private:
	std::vector<LockonedTarget> m_lockonRangeEnemy;
	LockonedTarget* m_lockonedEnemy;
	
	Texture* m_texLockoned;
	Texture* m_texLockonRange;

public:
	Enemy* GetTarget() { return m_lockonedEnemy ? m_lockonedEnemy->enemy : nullptr; }

	void Lockon(std::vector<Enemy*>* targetContainer);
	void RenderLockonEffect(LPD3DXSPRITE sprite);

public:
	Lockoner();
	~Lockoner();
};

