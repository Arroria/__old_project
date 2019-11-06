#pragma once
#include "Singleton.h"
class Score :
	public Singleton<Score>
{
private:
	UINT m_highScore;
	UINT m_nowScore;

public:
	void InsertScore(const int& score) { if ((int)m_nowScore + score >= 0) m_nowScore += score; else m_nowScore = 0; }
	UINT GetScore() { return m_nowScore; }
	UINT GetHighScore() { return m_highScore; }
	void ScoreSave() { if (m_nowScore > m_highScore) m_highScore = m_nowScore; }

public:
	Score();
	~Score();
};

