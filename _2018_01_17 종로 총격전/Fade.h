#pragma once
class Fade
{
private:
	bool m_isFadeIn;
	UINT m_time;

public:
	bool IsEnd();
	bool IsFadeIn() { return m_isFadeIn; }

public:
	void Initialize(const bool& isFadeIn);
	void Update();
	void Render(const D3DXVECTOR3& cameraPosition, D3DXVECTOR3 cameraSize);
	void Release();

public:
	Fade();
	~Fade();
};

