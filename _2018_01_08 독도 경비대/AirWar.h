#pragma once

class Jet;
class AI_Jet;
class Helicopter;
class AI_Helicopter;
class Vulkan;
class Kaga;

class AirWar
	: public BaseScene
{
private:
	std::vector<ASE*> m_loadedASE;

	Jet*	m_playerJet;
	UINT m_playerLife;
	LPDIRECT3DTEXTURE9 m_texPlayerHealthBar;
	LPDIRECT3DTEXTURE9 m_texPlayerHealthGauge;
	LPDIRECT3DTEXTURE9 m_texStageClear;
	LPDIRECT3DTEXTURE9 m_texStageFailed;

	Helicopter* m_dummyPlayer;

	std::vector<AI_Jet*> m_enemyJet;
	std::vector<AI_Helicopter*> m_enemyHelicopter;

	Kaga*	m_kaga;
	LPDIRECT3DTEXTURE9 m_texKagaHealthBar;
	LPDIRECT3DTEXTURE9 m_texKagaHealthGauge;

	std::vector<Vulkan*> m_playerAttack;
	std::vector<Vulkan*> m_enemyAttack;


	ASE*	m_mapModel;

	//
	UINT m_aiSpawnCount;

	//Camera
	Camera	m_playerCamera;
	D3DXQUATERNION m_cameraRotationQ;

	bool m_prevCameraLookAtBack;
	float m_prevCameraEyeVectorLength;
	float m_prevCameraEyeVectorAngle;

	UINT	m_cameraMod;

	//TEMP//
	ASE*	m_tempMapModel;

private:
	void SetPlayerCamera();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;

public:
	AirWar();
	~AirWar();
};

