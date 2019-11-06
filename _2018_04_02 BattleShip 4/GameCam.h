#pragma once
class GameCam
{
private:
	bool m_playerControl;

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_at;
	float m_dist;
	D3DXVECTOR2 m_rot;

	D3DXVECTOR3 m_objAt;
	float m_objDist;
	const float m_objRotX;

	UINT m_camShake;
	D3DXVECTOR3 m_shaking;

public:
	void PlayerControl() { m_playerControl = true; }
	void AutoControl() { m_playerControl = false; }

	void SetObjAt(const D3DXVECTOR3& at)	{ if (!m_playerControl) m_objAt = at; }
	void SetObjDist(const float& dist)		{ if (!m_playerControl) m_objDist = dist; }

	void SetCamShake(const UINT& shake) { if (m_camShake < shake) m_camShake = shake; }

	bool IsPlayerControl() { return m_playerControl; }

public:
	void Update();
	void Render();

public:
	GameCam();
	~GameCam();
};

