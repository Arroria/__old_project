#pragma once
class AIPlayer
{
private:
	std::array<bool, 100> m_attackData;
	std::list<POINT> m_hitData;
	std::list<POINT> m_scanData;

	bool m_isEven;

private:
	bool NextToHit(POINT& out);
	bool FindGrid(POINT& out, const bool& isEven);

public:
	void PutAttackData	(const POINT& data);
	void PutHitData		(const POINT& data);
	void PutScanData	(const POINT& data);

public:
	bool Attack(POINT& out);

public:
	AIPlayer();
	~AIPlayer();
};

