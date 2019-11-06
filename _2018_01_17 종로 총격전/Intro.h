#pragma once
#include "SceneManager.h"
class Intro :
	public Scene
{
private:
	float m_position;
	float m_scale;

	UINT m_counter;

public:
	void Initialize() override;
	void Update() override;
	void Render() override;
	void Release() override;

public:
	Intro();
	~Intro();
};

