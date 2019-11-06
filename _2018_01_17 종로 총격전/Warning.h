#pragma once
class Warning
{
private:
	UINT m_time;

public:
	bool PleaseDeleteThis() { return m_time > 300; }

public:
	void Initialize();
	void Update();
	void Render(const D3DXVECTOR3& cameraPosition);
	void Release();

public:
	Warning();
	~Warning();
};

