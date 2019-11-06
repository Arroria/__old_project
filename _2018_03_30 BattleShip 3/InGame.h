#pragma once
#include "Scene.h"

class Map;
class Ship;

class Lockon;
class Missile;
class Scanner;

class Effect;


class GameAI;

class InGame :
	public Scene
{
private:
	GameAI* m_aiPlayer;
	bool m_isWar;

	//�÷��̾�
	Map* m_playerMap;
	Map* m_enemyMap;
	std::array<Ship*, 5> m_playerShip;
	std::array<Ship*, 5> m_enemyShip;



	//Ŀ��
	POINT m_cursor;
	bool m_cursorInPlayerMap;
	bool m_cursorLookAtY;

	//��ġ���
	UINT m_selectedShip;
	std::array<bool, 5> m_isShipPlaced;
	bool m_isShipCanPlace;

	//���ø��
	bool m_isReadyToAttack;
	bool m_isPlayerTurn;
	std::array<bool, 4> m_isItemUsed;
	UINT m_selectedItem;
	UINT m_turnCount;

	std::list<Lockon*> m_listLockon;
	std::list<Missile*> m_listMissile;
	Scanner* m_scanner;
	std::list<Effect*> m_listEffect;



private:
	bool IsShipCanPlace	(Ship* target, const POINT& pos, const bool& isLookAtY, const bool& byPlayer);
	void ShipPlace		(Ship* target, const POINT& pos, const bool& isLookAtY, const bool& byPlayer);
	void ShipRandomPlace(const bool& byPlayer);
	

	Ship* GetRandomLiveShip(const bool& forPlayer);

	void DefaultAttack	(Ship* launcher, const POINT& pos, const bool& byPlayer);
	void LineAttack		(Ship* launcher, const UINT& line, const bool& isLookAtY, const bool& byPlayer);
	void RandomAttack	(Ship* launcher, const bool& byPlayer);
	void _33Attack		(Ship* launcher, const POINT& pos, const bool& byPlayer);
	void Scanning		(Ship* launcher, const POINT& pos, const bool& byPlayer);

public:
	void Initialize() override;
	void Update() override;
	void Render() override;
	void Release() override;

public:
	InGame();
	~InGame();
};

