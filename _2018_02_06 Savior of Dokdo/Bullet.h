#pragma once

class Enemy;

class Bullet
{
private:
	D3DXVECTOR3 m_position;
	D3DXQUATERNION m_rotation;

	D3DXVECTOR3 m_targetPosition;
	D3DXVECTOR3 m_direction;


	Enemy* m_target;

	bool m_isExpired;

public:
	bool IsExpired() { return m_isExpired; }

public:
	void Initialize();
	void Update();
	void Render(ASE* model);

public:
	Bullet(const D3DXVECTOR3& position, Enemy* target);
	~Bullet();
};

