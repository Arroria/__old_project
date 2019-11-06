#pragma once
class Intro
	: public Scene
{
private:
	bool m_scene2;
	UINT m_noise;

public:
	void Initialize() override;
	void Update() override;
	void Render() override;
	void Release() override;

public:
	Intro();
	~Intro();
};

