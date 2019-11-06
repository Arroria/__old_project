#pragma once

class Map;
class Ship;
class GameUI;

class Effect;
class Missile;

class GameAI;

class InGame
	: public Scene
{
private:
	bool m_settingTime;

	UINT m_view;

	/* SettingTime */
	std::array<bool, 5> m_shipSetting;
	int m_settingCursor;
	bool m_settingLookAtRight;
	POINT m_settingShipPos;
	bool m_isCanPlace;
	POINT m_prevCursorPos;

	/* PlayTime */
	bool m_playerTurn;
	UINT m_enemySeleteDelay;
	UINT m_turnChangeDelay;
	LPD3DXFONT m_scoreFont;
	UINT m_turn;

	std::list<Missile*> m_missile;

	/* End */
	bool m_victory;
	bool m_playerWin;
	UINT m_victoryFrame;


	/* Player */
	GameUI* m_uiPlayer;
	Map* m_mapPlayer;
	std::array<Ship*, 5> m_shipPlayer;
	std::array<bool, 4> m_itemPlayer;
	bool m_prevAttackHit;

	/* Enemy */
	GameAI* m_ai;
	GameUI* m_uiEnemy;
	Map* m_mapEnemy;
	std::array<Ship*, 5> m_shipEnemy;
	std::array<bool, 4> m_itemEnemy;



	/* Effect */
	std::list<Effect*> m_effect;
	std::list<Effect*> m_marker;

	Channel* m_bgm;

private:
	void SettingTimeUpdate();
	void RunTimeUpdate();
	void SettingTimeRender();
	void RunTimeRender();

public:
	void Initialize() override;
	void Update() override;
	void Render() override;
	void Release() override;

public:
	InGame();
	~InGame();
};

