#pragma once
class OBBCollision;
class Helicopter
{
private:
	D3DXVECTOR3 m_position;
	D3DXQUATERNION m_tiltRotation;
	D3DXQUATERNION m_objectiveRotation;
	D3DXQUATERNION m_identityRotation;

	float m_engineOutput;
	float m_objectiveEngineOutput;
	
	ASE* m_bodyModel;
	ASE* m_topRotorModel;
	ASE* m_tailRotorModel;

	OBBCollision* m_obb;

private:
	D3DXVECTOR3 GetAxis (const D3DXVECTOR3& defaultAxis) const;

	void Rotate(const D3DXVECTOR3& axis, const float& degree);

public:
	D3DXVECTOR3 GetPosition()	const { return m_position; }
	D3DXQUATERNION GetRotation() const { return m_tiltRotation; }
	D3DXVECTOR3 GetRollAxis()	const { return GetAxis(D3DXVECTOR3(0, 0, 1)); }
	D3DXVECTOR3 GetPitchAxis()	const { return GetAxis(D3DXVECTOR3(0, 1, 0)); }
	D3DXVECTOR3 GetRawAxis()	const { return GetAxis(D3DXVECTOR3(1, 0, 0)); }

	OBBCollision& GetOBB() { return *m_obb; }

	void SetPosition(const D3DXVECTOR3& position) { m_position = position; }
	void SetRotation(const D3DXQUATERNION& rotation) { m_tiltRotation = rotation; }

	void Pull(const bool& up, const float& power)		{ Rotate(GetRawAxis(),	D3DXToRadian(power) * (up ? -1 : 1)); }
	void Tilt(const bool& right, const float& power)	{ Rotate(GetRollAxis(), D3DXToRadian(power) * (right ? -1 : 1)); }
	void Turn(const bool& right, const float& power);
	void Engine(const float& power)	{ m_objectiveEngineOutput += power; }

public:
	void Initialize();
	void Update();
	void Render();
	void Release();

	
public:
	Helicopter(ASE* bodyModel, ASE* topRotorModel, ASE* tailRotorModel);
	~Helicopter();
};

