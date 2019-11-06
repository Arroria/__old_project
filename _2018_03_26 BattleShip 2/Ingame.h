#pragma once
#include "Scene.h"

class Map;
class Ship;
class GameUI;

class Lockon;
class Missile;
class Scan;

class Effect;

class Ingame :
	public Scene
{
private:
	/* Control */
	POINT	m_myCursorPos;

		/* Camera */
		float m_camDistance;
		float m_camObjDist;
		float m_camRotX;
		float m_camRotY;


		/* Setting */
		bool	m_myDirY; // use in playMode only LineAttack Skil
		UINT	m_myCursorShip;
		std::array<bool, 5> m_shipPlaced;
		UINT	m_settingSkillMouseOn;

		/* Play */
		bool m_cursorInPlayerMap;
		UINT m_itemSelect;
		UINT m_itemMouseOn;

		bool m_isPlayerTurn;
		bool m_isTurnAttacked;
		UINT m_turnStay;
	

	/* Global */
	Map* m_playerMap;
	Map* m_enemyMap;
	std::array<Ship*, 5> m_playerShip;
	std::array<Ship*, 5> m_enemyShip;
	GameUI* m_playerUI;
	GameUI* m_enemyUI;
	std::array<bool, 4> m_playerItem;
	std::array<bool, 4> m_enemyItem;
	bool m_isPlaying;

	/* Play */
	std::list<Lockon*> m_listLockon;
	std::list<Missile*> m_listMissile;
	Scan* m_scan;

	std::list<Effect*> m_listEffect;

	UINT m_turnFrame;

	/* Victory */
	UINT m_gameEnd;
	bool m_playerWin;

	Channel* m_bgm;

private:
	void CameraControl();

	/* Setting */
	Ship* CursorShip() { return m_myCursorShip != -1 ? m_playerShip[m_myCursorShip] : nullptr; }

	void SelectCursorShip(); // 배치모드 배 선택 함수
	void AutoPlaceShip(); // 자동 배 배치
	void ResetPlaceShip(); // 배치된 배 전부 회수

	/* Skill */
	void LineAttack		(const bool& byPlayer);
	void RandomAttack	(const bool& byPlayer);
	void _33Attack		(const bool& byPlayer);
	void Scanner		(const bool& byPlayer);

	void LaunchMissile(const POINT& lockonPos, const bool& byPlayer, const UINT& stay = 0);

public:
	void Initialize() override;
	void Update() override;
	void Render() override;
	void Release() override;

public:
	Ingame();
	~Ingame();
};

