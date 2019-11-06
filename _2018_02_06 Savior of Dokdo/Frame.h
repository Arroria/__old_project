#pragma once
class Frame
{
protected:
	D3DXVECTOR3 m_position;
	D3DXQUATERNION m_rollRotation;
	D3DXQUATERNION m_yawRotation;

	float m_speed;

public:
	//
	D3DXVECTOR3		GetPosition()		const { return m_position; }
	D3DXVECTOR3		GetDirection()		const ;
	D3DXQUATERNION	GetRollRotation()	const { return m_rollRotation; }
	D3DXQUATERNION	GetYawRotation()	const { return m_yawRotation; }

	void SetPosition(const D3DXVECTOR3& position) { m_position = position; }
	
	//
	float	GetSpeed() const { return m_speed; }

	void	SetSpeed(const float& speed) { m_speed = speed; }

public:
	Frame();
	~Frame();
};

