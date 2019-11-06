#pragma once
#include "Scene.h"

class Map;
class Ship;
class AIPlayer;
class GameUI;
class GameCam;

class Lockoner;
class Missile;
class Scanner;

class Effect;

class InGame :
	public Scene
{
private:
	//G
	bool m_isWar;
	bool m_win;
	bool m_pWin;
	UINT m_winFrame;

	Map* m_playerMap;
	Map* m_enemyMap;
	std::array<Ship*, 5> m_playerShip;
	std::array<Ship*, 5> m_enemyShip;
	AIPlayer* m_aiPlayer;
	GameUI* m_playerUI;
	GameUI* m_enemyUI;
	GameCam* m_gameCam;

	POINT m_cursor;
	bool m_cursorLookAtY;
	bool m_cursorInPlayerMap;

	//settingTime
	UINT m_selectedShip;
	std::array<bool, 5> m_isShipPlaced;
	bool m_isCanPlace;

	UINT m_toolSelecting;

	//WarTime
	bool m_isPlayerTurn;
	bool m_isReadyToAttack;
	UINT m_turnCount;

	UINT m_itemSelecting;
	UINT m_selectedItem;
	std::array<bool, 4> m_isPlayerItemUsed;
	std::array<bool, 4> m_isEnemyItemUsed;

	std::list<Lockoner*>	m_listLockoner;
	std::list<Missile*>		m_listMissile;
	Scanner*				m_scanner;
	std::list<Effect*>		m_listEffect;



public:
	bool CanPlaceShip	(Ship* target, const POINT& pos, const bool& isLookAtY, const bool& byPlayer);
	void PlaceShip		(Ship* target, const POINT& pos, const bool& isLookAtY, const bool& byPlayer);
	void RandomPlaceShip(const bool& byPlayer);
	void ResetPlace		(const bool& byPlayer);


	Ship* GetRandomLiveShip(const bool& byPlayer);

	void DefaultAttack	(Ship* launcher, const POINT& attackPos,					const bool& byPlayer);
	void LineAttack		(Ship* launcher, const UINT& line, const bool& isLookAtY,	const bool& byPlayer);
	void RandomAttack	(Ship* launcher,											const bool& byPlayer);
	void _33Attack		(Ship* launcher, const POINT& attackPos,					const bool& byPlayer);
	void Scanning		(Ship* launcher, const POINT& attackPos,					const bool& byPlayer);
	void JUSTKILL		(Ship* launcher, const bool& byPlayer);


public:
	void Initialize() override;
	void Update() override;
	void Render() override;
	void Release() override;

public:
	InGame();
	~InGame();
};

