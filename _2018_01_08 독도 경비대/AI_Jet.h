#pragma once

class Jet;
class AI_Jet
{
private:
	Jet* m_me;
	Jet* m_player;

	ASE* m_model;

	bool m_attack;

public:
	void SetPosition(const D3DXVECTOR3& position);

public:
	void Initialize();
	void Update();
	void Render();
	void Release();

public:
	AI_Jet(ASE* model, Jet* player);
	~AI_Jet();
};

