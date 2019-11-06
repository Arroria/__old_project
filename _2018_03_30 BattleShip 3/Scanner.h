#pragma once
#include "Effect.h"
class Scanner
{
private:
	D3DXVECTOR3 m_pos;
	POINT m_lockonPos;
	bool m_byPlayer;

	UINT m_frame;

public:
	POINT GetLockonPos() { return m_lockonPos; }
	bool ByPlayer() { return m_byPlayer; }
	bool IsEnd();

public:
	void Update();
	void Render();

public:
	Scanner(const D3DXVECTOR3& pos, const POINT& lockonPos, const bool& byPlayer);
	~Scanner();
};
