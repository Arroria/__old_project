#pragma once

class Player;

class EnemyAttack
{
private:
	bool m_isExpired;

public:
	bool IsExpired() { return m_isExpired; }
	void Expire() { m_isExpired = true; }

public:
	virtual void Update(Player* player) PURE;
	virtual void Render() PURE;

public:
	EnemyAttack();
	~EnemyAttack();
};

