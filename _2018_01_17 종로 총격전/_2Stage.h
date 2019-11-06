#pragma once
class Player;
class Enemy;
class Attack;
class Grenade;
class APC;
class Item;

class BlockObject;

class Effect;
class Fade;
class Warning;

class _2Stage
	: public Scene
{
private:
	Player* m_player;
	std::vector<Enemy*> m_enemy;
	APC* m_apc;

	std::vector<Grenade*> m_grenade;
	std::vector<Attack*> m_playerAttack;
	std::vector<Attack*> m_enemyAttack;

	std::vector<BlockObject*> m_blockObject;

	std::vector<Item*> m_item;
	std::list<Effect*> m_effect;

	Collision_Line m_road;
	Collision_Line m_specialPlace;
	BlockObject* m_attackBlock;

	UINT m_timer;

	Warning* m_warning;
	Camera m_camera;

public:
	void Initialize() override;
	void Update() override;
	void Render() override;
	void Release() override;

public:
	_2Stage();
	~_2Stage();
};

