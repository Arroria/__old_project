#pragma once
class Vulkan;
class Jet
{
private:
	ASE*	m_model;

	D3DXVECTOR3 m_position;
	D3DXQUATERNION m_rotation;


	D3DXVECTOR3 m_direction;
	D3DXVECTOR3 m_pitchAxis;


	float m_speed;
	float m_engineOutput;

	std::vector<Vulkan*>* m_bulletContainer;
	UINT m_attackCooltime;

	OBBCollision* m_obb;

	UINT m_health;

private:
	void JetRotation(const D3DXVECTOR3& axis, const float& power);

public:
	void PullUp		(const float& power);
	void PullDown	(const float& power);
	void RotateLeft	(const float& power);
	void RotateRight(const float& power);
	void MoveLeft	(const float& power);
	void MoveRight	(const float& power);
	void Boost		(const float& power);
	void SlowDown	(const float& power);
	void Attack		(ASE* bulletASE);

	void SetPosition(const D3DXVECTOR3& position) { m_position = position; }
	void Attacked() { if (m_health) m_health--; }
	UINT GetHealth() { return m_health; }
	void SetHealth(const UINT& health) { m_health = health; }

	D3DXVECTOR3 GetPosition()		{ return m_position; }
	D3DXQUATERNION GetRotation()	{ return m_rotation; }
	D3DXVECTOR3 GetDirection()		{ return m_direction; }
	D3DXVECTOR3 GetPitchAxis()		{ return m_pitchAxis; }

	OBBCollision& GetOBB()			{ return *m_obb; }

public:
	void Initialize(std::vector<Vulkan*>* bulletContainer);
	void Update();
	void Render();
	void Release();

public:
	Jet(ASE* model);
	~Jet();
};

