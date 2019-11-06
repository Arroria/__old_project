#pragma once

class Player;
class Enemy;
class Attack;
class Grenade;
class SlowObject;
class BlockObject;
class Insergent;

class House;

class Effect;
class Fade;
class Item;

class Warning;


class _1Stage
	: public Scene
{
private:
	Player* m_player;
	std::vector<Enemy*> m_enemy;
	Insergent* m_insergent;

	std::vector<Grenade*> m_grenade;
	std::vector<Attack*> m_playerAttack;
	std::vector<Attack*> m_enemyAttack;
	
	std::vector<SlowObject*> m_slowObject;
	std::vector<BlockObject*> m_blockObject;


	std::vector<House*> m_house;

	std::vector<Item*> m_item;
	std::list<Effect*> m_effect;


	UINT m_stageProgress;
	UINT m_enemyRush;
	UINT m_insergentCountwatisfackingthesdasdsadaada;
	UINT m_playerAutoMoving;

	Fade* m_fade;
	Warning* m_warning;
	Camera m_camera;

public:
	void Initialize() override;
	void Update() override;
	void Render() override;
	void Release() override;

public:
	_1Stage();
	~_1Stage();
};

