#pragma once
class MainMenu
	: public Scene
{
private:
	UINT m_state;
	UINT m_nextState;

	bool m_runAnime;
	UINT m_mainFrame;
	UINT m_mainFrameOST;

	UINT m_btnFrame;

	UINT m_logoAnime;
	bool m_logoOn;

	LPD3DXFONT m_scoreFont;
	Channel* m_channel;

public:
	void Initialize() override;
	void Update() override;
	void Render() override;
	void Release() override;

public:
	MainMenu();
	~MainMenu();
};

