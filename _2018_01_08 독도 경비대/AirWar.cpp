#include "stdafx.h"
#include "AirWar.h"

#include "Jet.h"
#include "AI_Jet.h"
#include "Helicopter.h"
#include "AI_Helicopter.h"
#include "Vulkan.h"
#include "Kaga.h"

#include "OBBCollision.h"

AirWar::AirWar()
	: m_playerJet(nullptr)
	, m_playerLife(4)
	, m_texPlayerHealthBar(nullptr)
	, m_texPlayerHealthGauge(nullptr)
	, m_texStageClear(nullptr)
	, m_texStageFailed(nullptr)
	, m_dummyPlayer(nullptr)

	, m_kaga(nullptr)
	, m_texKagaHealthBar(nullptr)
	, m_texKagaHealthGauge(nullptr)

	, m_mapModel(nullptr)
	, m_cameraRotationQ(0, 0, 0, 1)

	, m_prevCameraLookAtBack(false)
	, m_prevCameraEyeVectorLength(0)
	, m_prevCameraEyeVectorAngle(0)

	, m_cameraMod(0)

	, m_aiSpawnCount(600)
{
}
AirWar::~AirWar()
{
}



void AirWar::Initialize()
{
	auto LoadTexture = [](LPDIRECT3DTEXTURE9& tex, const std::wstring& path)
	{
		D3DXCreateTextureFromFileExW(DEVICE, path.c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DXCOLOR(0, 0, 0, 0), nullptr, nullptr, &tex);
	};
	LoadTexture(m_texPlayerHealthBar, L"./Resource/sprite/healthBar.png");
	LoadTexture(m_texPlayerHealthGauge, L"./Resource/sprite/healthGauge.png");
	LoadTexture(m_texStageClear, L"./Resource/sprite/stageClear.png");
	LoadTexture(m_texStageFailed, L"./Resource/sprite/stageFailed.png");
	LoadTexture(m_texPlayerHealthBar, L"./Resource/sprite/enemyHealthBar.png");
	LoadTexture(m_texKagaHealthGauge, L"./Resource/sprite/enemyHealthGauge.png");

	ASELoader aseLoader;
	m_loadedASE.resize(10);
	m_loadedASE[0] = aseLoader.LoadASE(L"./Resource/model/map", L"map.ase");
	m_loadedASE[1] = aseLoader.LoadASE(L"./Resource/model/jet", L"jet.ase");
	m_loadedASE[2] = aseLoader.LoadASE(L"./Resource/model", L"test4.ase");
	m_loadedASE[3] = aseLoader.LoadASE(L"./Resource/model/sea", L"sea.ase");
	m_loadedASE[4] = aseLoader.LoadASE(L"./Resource/model/helicopter", L"helicopter.ase");
	m_loadedASE[5] = aseLoader.LoadASE(L"./Resource/model/vulcan", L"vulcan.ase");
	m_loadedASE[6] = aseLoader.LoadASE(L"./Resource/model/kaga", L"kaga.ase");

	m_mapModel = m_loadedASE[0];
	m_tempMapModel = m_loadedASE[2];


	m_playerJet = new Jet(m_loadedASE[1]);
	m_playerJet->Initialize(&m_playerAttack);
	m_dummyPlayer = new Helicopter(m_loadedASE[4], nullptr, nullptr);
	m_dummyPlayer->Initialize();

	m_kaga = new Kaga(&m_enemyHelicopter, &m_enemyAttack, m_loadedASE[5]);
	D3DXQUATERNION kagaQ;
	D3DXQuaternionRotationAxis(&kagaQ, &D3DXVECTOR3(0, 1, 0), D3DXToRadian(rand() % 360));
	m_kaga->Initialize(m_playerJet, m_loadedASE[4], m_loadedASE[6], kagaQ);


	//AI summon
	for (UINT i = 0; i < 4; i++)
	{
		AI_Helicopter* ai = new AI_Helicopter(m_playerJet, &m_enemyAttack, m_loadedASE[5]);
		ai->Initialize(m_loadedASE[4], nullptr, nullptr);
		ai->SetPosition(D3DXVECTOR3(i < 2 ? 1000 : -1000, 50, i % 2 ? 1000 : -1000));
		m_enemyHelicopter.push_back(ai);
	}


	m_playerCamera.SetCameraProjection(D3DXToRadian(60), 1280. / 960., .1, 10000);
	m_playerCamera.SetCameraView(nullptr, nullptr, &D3DXVECTOR3(0, 1, 0));
}

void AirWar::Update()
{
	if (m_playerLife || m_playerJet->GetHealth())
	{
		//PlayerControl
		CONST double FrontRotation = 1.5;
		CONST double WingRotation = 3;
		CONST double TopRotation = 0.5;

		if (GetAsyncKeyState(VK_NUMPAD5))	m_playerJet->PullUp(FrontRotation);
		if (GetAsyncKeyState(VK_NUMPAD8))	m_playerJet->PullDown(FrontRotation);
		if (GetAsyncKeyState(VK_NUMPAD4))	m_playerJet->RotateLeft(WingRotation);
		if (GetAsyncKeyState(VK_NUMPAD6))	m_playerJet->RotateRight(WingRotation);
		if (GetAsyncKeyState('A'))			m_playerJet->MoveLeft(TopRotation);
		if (GetAsyncKeyState('D'))			m_playerJet->MoveRight(TopRotation);
		if (GetAsyncKeyState('W'))			m_playerJet->Boost(0);
		if (GetAsyncKeyState('S'))			m_playerJet->SlowDown(0);
		if (GetAsyncKeyState(VK_SPACE))		m_playerJet->Attack(m_loadedASE[5]);
	}

	///if (GetAsyncKeyState(VK_NUMPAD5))	m_dummyPlayer->Pull(true, FrontRotation);
	///if (GetAsyncKeyState(VK_NUMPAD8))	m_dummyPlayer->Pull(false, FrontRotation);
	///if (GetAsyncKeyState(VK_NUMPAD4))	m_dummyPlayer->Tilt(false, WingRotation);
	///if (GetAsyncKeyState(VK_NUMPAD6))	m_dummyPlayer->Tilt(true, WingRotation);
	///if (GetAsyncKeyState('A'))			m_dummyPlayer->Turn(false, TopRotation);
	///if (GetAsyncKeyState('D'))			m_dummyPlayer->Turn(true, TopRotation);
	///if (GetAsyncKeyState('W'))			m_dummyPlayer->Engine(0.01);
	///if (GetAsyncKeyState('S'))			m_dummyPlayer->Engine(-0.01);

	//Function
	auto PlayerDestroy = [](Jet* player, UINT& life)
	{
		if (life)
		{
			life--;
			player->SetPosition(D3DXVECTOR3(0, 100, 0));
			player->SetHealth(10);
		}
		else
			player->SetHealth(0);
	};

	//summon AI
	if (!m_aiSpawnCount)
	{
		m_aiSpawnCount = 300;
		if (rand() % 3 == 0)
		{
			AI_Helicopter* ai = new AI_Helicopter(m_playerJet, &m_enemyAttack, m_loadedASE[5]);
			ai->Initialize(m_loadedASE[4], nullptr, nullptr);
			ai->SetPosition(D3DXVECTOR3(-2000 + (rand() % 5) * 1000, 300, -2000 + (rand() % 5) * 1000));
			m_enemyHelicopter.push_back(ai);
		}
	}
	m_aiSpawnCount--;

	//Enemy Attack Update
	for (auto iter = m_enemyAttack.begin(); iter != m_enemyAttack.end();)
	{
		(*iter)->Update();

		if ((*iter)->GetOBB().CalculateCollision(m_playerJet->GetOBB()))
		{
			(*iter)->SetDelete();
			m_playerJet->Attacked();
			if (!m_playerJet->GetHealth() && m_playerLife)
				PlayerDestroy(m_playerJet, m_playerLife);
		}

		if ((*iter)->GetDelete())
		{
			SAFE_RELEASE_DELETE(*iter);
			iter = m_enemyAttack.erase(iter);
		}
		else
			iter++;
	}

	//Player Update
	m_playerJet->Update();
	if (m_playerJet->GetPosition().y < 0 ||
		m_playerJet->GetOBB().CalculateCollision(m_kaga->GetOBB()))
		PlayerDestroy(m_playerJet, m_playerLife);
	else
	{
		for (auto iter = m_enemyHelicopter.begin(); iter != m_enemyHelicopter.end(); iter++)
		{
			if (m_playerJet->GetOBB().CalculateCollision((*iter)->GetOBB()))
			{
				PlayerDestroy(m_playerJet, m_playerLife);
				SAFE_RELEASE_DELETE(*iter);
				iter = m_enemyHelicopter.erase(iter);
				break;
			}
		}
	}

	//Dummy Camera
	///m_dummyPlayer->Update();

	//KagaUpdate
	if (m_kaga->GetHelath() && (m_playerLife || m_playerJet->GetHealth()))
	{
		m_kaga->Update();
		if (D3DXVec3Length(&m_kaga->GetPosition()) < 100)
		{
			m_playerLife = 0;
			PlayerDestroy(m_playerJet, m_playerLife);
		}
	}

	//Player Attack Update
	for (auto iter = m_playerAttack.begin(); iter != m_playerAttack.end();)
	{
		(*iter)->Update();

		for (auto& iterE : m_enemyHelicopter)
		{
			if ((*iter)->GetOBB().CalculateCollision(iterE->GetOBB()))
			{
				iterE->HealthDown();
				(*iter)->SetDelete();
			}
		}
		if ((*iter)->GetOBB().CalculateCollision(m_kaga->GetOBB()))
		{
			m_kaga->Attacked();
			std::cout << m_kaga->GetHelath() << std::endl;
			(*iter)->SetDelete();
		}

		if ((*iter)->GetDelete())
		{
			SAFE_RELEASE_DELETE(*iter);
			iter = m_playerAttack.erase(iter);
		}
		else
			iter++;
	}

	///for (auto& iter : m_enemyJet)
	///	iter->Update();

	//Enemy Update
	for (auto iter = m_enemyHelicopter.begin(); iter != m_enemyHelicopter.end();)
	{
		(*iter)->Update();
		if ((*iter)->GetDelete())
		{
			SAFE_RELEASE_DELETE(*iter);
			iter = m_enemyHelicopter.erase(iter);
		}
		else
			iter++;
	}

	//CameraUpdate
	if (m_playerLife || m_playerJet->GetHealth())
		SetPlayerCamera();
}

extern ASE* skybox;
D3DXMATRIX skyboxPos;
void AirWar::Render()
{
	m_playerCamera.MatrixInput(DEVICE);
	m_playerJet->Render();
	m_dummyPlayer->Render();
	m_kaga->Render();
	for (auto& iter : m_enemyJet)
		iter->Render();
	for (auto& iter : m_enemyHelicopter)
		iter->Render();
	for (auto& iter : m_playerAttack)
		iter->Render();
	for (auto& iter : m_enemyAttack)
		iter->Render();


	LPD3DXSPRITE sp;
	D3DXCreateSprite(DEVICE, &sp);
	sp->Begin(D3DXSPRITE_ALPHABLEND);
	{
		D3DXMATRIX t;
		D3DXMatrixTranslation(&t, 30, 860, 0);
		sp->SetTransform(&t);
		sp->Draw(m_texPlayerHealthBar, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, 1));

		for (UINT i = 0; i < m_playerLife + (m_playerJet->GetHealth() ? 1 : 0); i++)
		{
			D3DXMatrixTranslation(&t, 160 + i * 50, 894, 0);
			sp->SetTransform(&t);
			sp->Draw(m_texPlayerHealthGauge, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, 1));
		}

		if (!m_playerLife && !m_playerJet->GetHealth())
		{
			D3DXMatrixTranslation(&t, (1280 - 602) * 0.5f, (960 - 115) * 0.5f, 0);
			sp->SetTransform(&t);
			sp->Draw(m_texStageFailed, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, 1));
		}

		if (!m_kaga->GetHelath() && (m_playerLife || m_playerJet->GetHealth()))
		{
			D3DXMatrixTranslation(&t, (1280 - 698) * 0.5f, (960 - 128) * 0.5f, 0);
			sp->SetTransform(&t);
			sp->Draw(m_texStageClear, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, 1));
		}
	}
	sp->End();
	SAFE_RELEASE(sp);


	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);
	m_mapModel->Render(identity);
	//m_tempMapModel->Render(identity);


	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	skybox->Render(skyboxPos);
	DEVICE->SetRenderState(D3DRS_LIGHTING, true);


	CONST int seaSize = 150;
	D3DXVECTOR3 pPos = m_playerJet->GetPosition();
	pPos = D3DXVECTOR3
		(((int)pPos.x / seaSize) * seaSize,
		0,
		((int)pPos.z / seaSize) * seaSize);
	for (int i = -10; i < 10; i++)
	{
		for (int j = -10; j < 10; j++)
		{
			D3DXMATRIX seaPosition;
			D3DXMatrixTranslation(&seaPosition, pPos.x + seaSize * i, pPos.y, pPos.z + seaSize * j);
			m_loadedASE[3]->Render(seaPosition);
		}
	}
}

void AirWar::Release()
{
	for (auto& iter : m_enemyJet)
	{
		SAFE_RELEASE_DELETE(iter);
	}
	for (auto& iter : m_enemyHelicopter)
	{
		SAFE_RELEASE_DELETE(iter);
	}
	
	SAFE_RELEASE_DELETE(m_playerJet);
	SAFE_RELEASE_DELETE(m_dummyPlayer);
	SAFE_RELEASE_DELETE(m_kaga);

	SAFE_RELEASE(m_texPlayerHealthBar);
	SAFE_RELEASE(m_texPlayerHealthGauge);
	SAFE_RELEASE(m_texKagaHealthBar);
	SAFE_RELEASE(m_texKagaHealthGauge);
}



#define PerpendicularRotationSpeed	0.4f
#define HorizonRotationSpeed		0.1f
void AirWar::SetPlayerCamera()
{
	//Camera State
	bool cameraLookAtBack = GetSingleton(InputManager)->IsKeyPressed('C');

	std::pair<float, float> viewData[4];
	viewData[0] = std::make_pair(45, 1);
	viewData[1] = std::make_pair(10, 3);
	viewData[2] = std::make_pair(15, 6);
	viewData[3] = std::make_pair(15, 10);

	if (GetSingleton(InputManager)->IsKeyDown('V'))
	{
		m_cameraMod++;
		if (m_cameraMod >= 4)
			m_cameraMod = 0;
	}

	float cameraEyeAngle = m_prevCameraEyeVectorAngle + (viewData[m_cameraMod].first - m_prevCameraEyeVectorAngle) * 0.1f;
	float cameraEyeVectorLength = m_prevCameraEyeVectorLength + (viewData[m_cameraMod].second - m_prevCameraEyeVectorLength) * 0.1f;


	//Camera Direction Setting
	///D3DXVECTOR3 targetPosition =		m_dummyPlayer->GetPosition();
	///D3DXQUATERNION objectiveRotationQ = m_dummyPlayer->GetRotation();
	D3DXVECTOR3 targetPosition =		m_playerJet->GetPosition();
	D3DXQUATERNION objectiveRotationQ = m_playerJet->GetRotation();
	D3DXVECTOR3 objectiveDirectionUp(0, 1, 0);
	D3DXVECTOR3 objectiveDirectionAt(0, 0, (cameraLookAtBack ? -1 : 1));
	D3DXVECTOR3 objectiveDirectionEye;
	//relativeEye =
	{
		D3DXMATRIX tempRotationMatrix;
		D3DXMatrixRotationX(&tempRotationMatrix, (cameraLookAtBack ? -1 : 1) * D3DXToRadian(cameraEyeAngle));
		D3DXVec3TransformCoord(&objectiveDirectionEye, &D3DXVECTOR3(0, 0, (cameraLookAtBack ? 1 : -1)), &tempRotationMatrix);
	}


	//Camera Rotation Setting
	m_prevCameraLookAtBack == cameraLookAtBack ?
		*D3DXQuaternionSlerp(&m_cameraRotationQ, &m_cameraRotationQ, &objectiveRotationQ, 0.1f) :
		m_cameraRotationQ = objectiveRotationQ;

	//Camera Objective View Direction Setting
	D3DXMATRIX cameraRotationMatrix;
	D3DXMatrixRotationQuaternion(&cameraRotationMatrix, &m_cameraRotationQ);
	D3DXVec3TransformCoord(&objectiveDirectionUp, &objectiveDirectionUp, &cameraRotationMatrix);
	D3DXVec3TransformCoord(&objectiveDirectionAt, &objectiveDirectionAt, &cameraRotationMatrix);
	D3DXVec3TransformCoord(&objectiveDirectionEye, &objectiveDirectionEye, &cameraRotationMatrix);

	//Camera View Direction Setting
	D3DXVECTOR3 cameraUpDirection;
	D3DXVECTOR3 cameraDirection;
	D3DXVECTOR3 cameraRelativePosition;
	///	if (m_prevCameraLookAtBack == cameraLookAtBack)
	///	{
	///		D3DXVec3Lerp(&cameraUpDirection,		&m_playerCamera.GetCameraUp(),	&objectiveDirectionUp,	0.05f);
	///		D3DXVec3Lerp(&cameraDirection,			&m_prevCameraDirection,			&objectiveDirectionAt,	0.05f);
	///		D3DXVec3Lerp(&cameraRelativePosition,	&m_prevCameraRelativePosition,	&objectiveDirectionEye,	0.05f);
	///		D3DXVec3Normalize(&cameraUpDirection, &cameraUpDirection);
	///		D3DXVec3Normalize(&cameraDirection, &cameraDirection);
	///		D3DXVec3Normalize(&cameraRelativePosition, &cameraRelativePosition);
	///	}
	///	else
	{
		cameraUpDirection = objectiveDirectionUp;
		cameraDirection = objectiveDirectionAt;
		cameraRelativePosition = objectiveDirectionEye;
	}

	//Camera View Setting
	m_playerCamera.SetCameraView(
		&(targetPosition + cameraRelativePosition * cameraEyeVectorLength),
		&(targetPosition + cameraDirection * 1000),
		&cameraUpDirection);

	m_prevCameraLookAtBack = cameraLookAtBack;
	m_prevCameraEyeVectorAngle = cameraEyeAngle;
	m_prevCameraEyeVectorLength = cameraEyeVectorLength;



	D3DXVECTOR3 cameraEye = m_playerCamera.GetCameraPosition();
	D3DXMATRIX s;
	D3DXMatrixScaling(&s, 23, 23, 23);
	D3DXMatrixTranslation(&skyboxPos, cameraEye.x, cameraEye.y, cameraEye.z);
	skyboxPos = s * skyboxPos;

}
