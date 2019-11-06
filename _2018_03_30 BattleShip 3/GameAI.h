#pragma once
class InGame;
class GameAI
{
	friend InGame;
private:
	std::array<bool, 100> m_attackData;
	std::list<POINT> m_hitData;
	
	std::array<bool, 4> m_item;
	std::list<POINT> m_scanData;
	
	bool m_isEven;
	UINT m_attackCount;

private:
	bool NextToHitFine(POINT& output);
	bool GridFind(POINT& output, const bool& isEven);


public:
	bool Attack(POINT& output);
	
	bool DeclarationUseItem(UINT& itemType);
	bool LineAttack		(UINT & line, bool& isLookAtY);
	bool _33Attack		(POINT& output);
	bool Scanning		(POINT& output);

	void PutHitData(const POINT& hitPos);
	void PutScanData(const POINT& scanPos);
	void PutAttackData(const POINT& attackPos);
	
	void AttackCountUp() { m_attackCount++; }
	UINT GetAttackCount() { return m_attackCount; }

public:
	GameAI();
	~GameAI();
};

