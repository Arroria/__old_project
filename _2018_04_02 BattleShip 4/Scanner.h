#pragma once
class Scanner
{
private:
	POINT m_scanningPos;
	bool m_byPlayer;
	
	UINT m_frame;

	bool m_isUsed;

public:
	POINT GetScanningPos() { return m_scanningPos; }
	bool ByPlayer() { return m_byPlayer; }

	bool Complate();
	bool IsEnd();
	bool IsUsed() { return m_isUsed; }
	void SetUsed() { m_isUsed = true; }

public:
	void Update();
	void Render(const D3DXVECTOR3& pos);

public:
	Scanner(const POINT& scanningPos, const bool& byPlayer);
	~Scanner();
};

