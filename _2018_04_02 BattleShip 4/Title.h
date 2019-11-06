#pragma once
#include "Scene.h"
class Title :
	public Scene
{
private:
	bool m_introGo;
	std::pair<UINT, UINT> m_frame;

	bool m_nowNew;
	UINT m_state;

	UINT m_btnOn;
	UINT m_stateNext;

public:
	void Initialize() override;
	void Update() override;
	void Render() override;
	void Release() override;

public:
	Title();
	~Title();
};

