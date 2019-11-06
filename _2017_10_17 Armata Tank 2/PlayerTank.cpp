#include "stdafx.h"
#include "PlayerTank.h"

#include "PlayerAdmin.h"
#include "PlayerShell.h"

#define GameDeltaTime (g_processManager->GetDeltaTime())

#define TankMoveSpeed 5 * 10
#define TankTurnSpeed 240

#define WeaponTurnSpeed 180

#define FireCoolTime_Cannon		0.8
#define FireCoolTime_AutoCannon	0.05
#define FireCoolTime_Rocket		1.2

PlayerTank::PlayerTank(const D3DXVECTOR3& spawnPoint, const float& durability, const float& armour)
	: BaseTank(spawnPoint, durability, armour)
	, m_playerAdmin(nullptr)

	, m_txWheel(nullptr)
	, m_txBody(nullptr)
	, m_bodyObjectiveDirection(0,-1,0)
	, m_bodyLookingDirection(0,-1,0)
	, m_bodyLookingRotation(0)
	
	, m_weaponObjectiveDirection(NULL, NULL, NULL)
	, m_weaponLookingDirection(NULL, NULL, NULL)
	, m_weaponLookingRotation(0)

	, m_txWeapon(nullptr)
	, m_playerWeaponType(PlayerTankWeaponType::Cannon)
	, m_fireCooltime(0)
{
	//Move
	m_txWheel = g_textureContainer->GetTexture(L"Tank_Wheel");
	m_txBody = g_textureContainer->GetTexture(L"Tank_Body");

	//Aiming
	POINT mousePos = GetSingleton(InputManager)->GetMousePos();
	m_weaponLookingDirection = D3DXVECTOR3(mousePos.x - m_position.x, mousePos.y - m_position.y, 0);
	D3DXVec3Normalize(&m_weaponLookingDirection, &m_weaponLookingDirection);
	m_weaponObjectiveDirection = m_weaponLookingDirection;

	m_weaponLookingRotation = D3DXToDegree(acos(D3DXVec3Dot(&D3DXVECTOR3(0, -1, 0), &m_weaponLookingDirection)));
	if (m_weaponLookingDirection.x < 0)
		m_weaponLookingRotation = 360 - m_weaponLookingRotation;

	//Attack
	m_txWeapon = g_textureContainer->GetTexture(L"Tank_Cannon");


	//CollisionBox
	float temp = 4;
	m_collisionSize = D3DXVECTOR2(temp, temp);
}
PlayerTank::~PlayerTank()
{
}



//Used 2D Default CarmeraView Coordinate system.
void PlayerTank::Move()
{
	//Set body objective rotation;
	D3DXVECTOR3 objectiveDirection(0, 0, 0);;
		 if (GetSingleton(InputManager)->IsKeyPressed('W'))objectiveDirection += D3DXVECTOR3(0, 1, 0);
	else if (GetSingleton(InputManager)->IsKeyPressed('S')) objectiveDirection -= D3DXVECTOR3(0, 1, 0);
		 if (GetSingleton(InputManager)->IsKeyPressed('A')) objectiveDirection -= D3DXVECTOR3(1, 0, 0);
	else if (GetSingleton(InputManager)->IsKeyPressed('D')) objectiveDirection += D3DXVECTOR3(1, 0, 0);
	if (objectiveDirection.x != 0 || objectiveDirection.y != 0 || objectiveDirection.z != 0)
		D3DXVec3Normalize(&m_bodyObjectiveDirection, &objectiveDirection);

	if (D3DXVec3Length(&(m_bodyObjectiveDirection - m_bodyLookingDirection)) < 0.001f)
	{
		//Body move
		if (objectiveDirection.x != 0 || objectiveDirection.y != 0 || objectiveDirection.z != 0)
			m_position += m_bodyObjectiveDirection * TankMoveSpeed * GameDeltaTime;
	}
	else
	{
		D3DXMATRIX projMat;
		g_processManager->GetDevice()->GetTransform(D3DTS_PROJECTION, &projMat);

		//Body turn
		D3DXVECTOR3 turnDir;
		D3DXVec3Cross(&turnDir, &m_bodyLookingDirection, &m_bodyObjectiveDirection);
		
		D3DXMATRIX matTankTurn;
		D3DXMatrixRotationZ(&matTankTurn, D3DXToRadian(TankTurnSpeed * GameDeltaTime) * (turnDir.z < 0 /* True == Clockwise Turn */ ? -1 : 1));
		D3DXVec3TransformCoord(&m_bodyLookingDirection, &m_bodyLookingDirection, &matTankTurn);
		D3DXVec3Normalize(&m_bodyLookingDirection, &m_bodyLookingDirection);
		//Weapon looking direction interaction
			D3DXVec3TransformCoord(&m_weaponLookingDirection, &m_weaponLookingDirection, &matTankTurn);
		
		//Turn correction
		D3DXVECTOR3 turnOver;
		D3DXVec3Cross(&turnOver, &m_bodyLookingDirection, &m_bodyObjectiveDirection);
		if (turnOver.z * turnDir.z < 0)
			m_bodyLookingDirection = m_bodyObjectiveDirection;
	}

	//Set body rotation
	m_bodyLookingRotation = D3DXToDegree(acos( D3DXVec3Dot(&D3DXVECTOR3(0, -1, 0), &m_bodyLookingDirection) ));
	if (m_bodyLookingDirection.x < 0)
		m_bodyLookingRotation = 360 - m_bodyLookingRotation;
}

void PlayerTank::Aiming()
{
	//Set weapon objective rotation;

	//2D ver
	POINT mousePos(GetSingleton(InputManager)->GetMousePos());
	///m_weaponObjectiveDirection = D3DXVECTOR3(mousePos.x - m_position.x, mousePos.y - m_position.y, 0);

	//3D ver!!!!!
	//Get Mouse Looking Position
	D3DXVECTOR3 vPickedPosition(0, 0, 0);
	{
		//Mouse Ray
		D3DXVECTOR3 rayOrigin(0, 0, 0);
		D3DXVECTOR3 rayDirection;

		//Get Ray At World
		D3DXMATRIX matView, matInvView;
		g_processManager->GetDevice()->GetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixInverse(&matInvView, 0, &matView);
		{
			POINT mousePos(GetSingleton(InputManager)->GetMousePos());
			
			//Get Ray At View
			D3DVIEWPORT9 viewPort;
			g_processManager->GetDevice()->GetViewport(&viewPort);
			D3DXMATRIX projectionMatrix;
			g_processManager->GetDevice()->GetTransform(D3DTS_PROJECTION, &projectionMatrix);

			rayDirection.x = (2.0f *  mousePos.x / viewPort.Width - 1) / projectionMatrix._11;
			rayDirection.y = (-2.0f * mousePos.y / viewPort.Height + 1) / projectionMatrix._22;
			rayDirection.z = 1.0f;
		}

		D3DXVec3TransformCoord(&rayOrigin, &rayOrigin, &matInvView);
		D3DXVec3TransformNormal(&rayDirection, &rayDirection, &matInvView);
		D3DXVec3Normalize(&rayDirection, &rayDirection);


		//Collision Check Ray for Plane(World)
		#define _Correction 10000
		#define _Y_Height 19
 		std::vector<D3DXVECTOR3> tempV;
		tempV.push_back(D3DXVECTOR3(-1 * _Correction, _Y_Height, +1 * _Correction));
		tempV.push_back(D3DXVECTOR3(+1 * _Correction, _Y_Height, +1 * _Correction));
		tempV.push_back(D3DXVECTOR3(-1 * _Correction, _Y_Height, -1 * _Correction));
		tempV.push_back(D3DXVECTOR3(+1 * _Correction, _Y_Height, -1 * _Correction));
		float u, v, d;
		if (D3DXIntersectTri(&tempV[0], &tempV[1], &tempV[3], &rayOrigin, &rayDirection, &u, &v, &d) ||
			D3DXIntersectTri(&tempV[0], &tempV[3], &tempV[2], &rayOrigin, &rayDirection, &u, &v, &d))
			vPickedPosition = rayOrigin + rayDirection * d;
		else
			return;
	}

	//Get Objective Position
	m_weaponObjectiveDirection = D3DXVECTOR3(vPickedPosition.x - m_position.x, vPickedPosition.z - m_position.y, 0);
	D3DXVec3Normalize(&m_weaponObjectiveDirection, &m_weaponObjectiveDirection);

	if (D3DXVec3Length(&(m_weaponObjectiveDirection - m_weaponLookingDirection)) > 0.001f)
	{
		//Weapon turn
		D3DXVECTOR3 turnDir;
		D3DXVec3Cross(&turnDir, &m_weaponLookingDirection, &m_weaponObjectiveDirection);

		D3DXMATRIX matTankTurn;
		D3DXMatrixRotationZ(&matTankTurn, D3DXToRadian(WeaponTurnSpeed * GameDeltaTime) * (turnDir.z < 0 /* True == Clockwise Turn */ ? -1 : 1));
		D3DXVec3TransformCoord(&m_weaponLookingDirection, &m_weaponLookingDirection, &matTankTurn);
		D3DXVec3Normalize(&m_weaponLookingDirection, &m_weaponLookingDirection);

		//Turn correction
		D3DXVECTOR3 turnOver;
		D3DXVec3Cross(&turnOver, &m_weaponLookingDirection, &m_weaponObjectiveDirection);
		if (turnOver.z * turnDir.z < 0)
			m_weaponLookingDirection = m_weaponObjectiveDirection;
	}

	//Set body rotation
	m_weaponLookingRotation = D3DXToDegree(acos(D3DXVec3Dot(&D3DXVECTOR3(0, -1, 0), &m_weaponLookingDirection)));
	if (m_weaponLookingDirection.x < 0)
		m_weaponLookingRotation = 360 - m_weaponLookingRotation;
}

void PlayerTank::Attack()
{
	m_fireCooltime -= GameDeltaTime;

	if (m_fireCooltime <= 0 && (GetSingleton(InputManager)->IsKeyPressed(VK_SPACE) || GetSingleton(InputManager)->IsKeyPressed(VK_LBUTTON)))
	{
		switch (m_playerWeaponType)
		{
		case PlayerTankWeaponType::Cannon:
			m_playerAdmin->CreateShell(new PlayerShell(m_position, m_weaponLookingDirection, PlayerShellType::Cannon));
			m_fireCooltime = FireCoolTime_Cannon;
			break;
		case PlayerTankWeaponType::AutoCannon:
		{
			D3DXVECTOR3 shellPos;
			m_playerAdmin->CreateShell(new PlayerShell(m_position, m_weaponLookingDirection, PlayerShellType::AutoCannon));
			//shellPos = D3DXVECTOR3(-m_weaponLookingDirection.y, m_weaponLookingDirection.x, 0);
			//m_playerAdmin->CreateShell(new PlayerShell(m_position + shellPos * 15, m_weaponLookingDirection, PlayerShellType::AutoCannon));
			//shellPos = D3DXVECTOR3(m_weaponLookingDirection.y, -m_weaponLookingDirection.x, 0);
			//m_playerAdmin->CreateShell(new PlayerShell(m_position + shellPos * 10, m_weaponLookingDirection, PlayerShellType::AutoCannon));
			m_fireCooltime = FireCoolTime_AutoCannon;
		}
			break;
		case PlayerTankWeaponType::Rocket:
			m_playerAdmin->CreateShell(new PlayerShell(m_position, m_weaponLookingDirection, PlayerShellType::Rocket));
			m_fireCooltime = FireCoolTime_Rocket;
			break;
		}
	}
}

bool PlayerTank::Render()
{
	D3DXMATRIX mat1, mat2, mat3;
	D3DXMatrixTranslation(&mat2, m_position.x, m_position.z + 5, m_position.y);
	D3DXMatrixRotationY(&mat3, -D3DXToRadian(m_bodyLookingRotation - 90));
	/// D3DXMatrixScaling(&mat1, m_scale.x * 4, m_scale.z * 4, m_scale.y * 4);
	{
		D3DXVECTOR3 scale(m_scale * 0.004f);
		D3DXMatrixScaling(&mat1, scale.x, scale.z, scale.y);
	}
	g_ase->GetAse(L"TankBody")->Render(mat1 * mat3 * mat2);
	D3DXMatrixRotationY(&mat3, -D3DXToRadian(m_weaponLookingRotation - 90));
	g_ase->GetAse(L"TankTurret")->Render(mat1 * mat3 * mat2);
	/// g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(mat1 * mat3 * mat2));
	/// g_processManager->GetDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	/// g_processManager->GetDevice()->SetTexture(0, m_txBody->d3dxTexture);
	/// g_processManager->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	/// struct temp
	/// {
	/// 	D3DXVECTOR3 p;
	/// 	D3DXVECTOR2 uv;
	/// 	temp(D3DXVECTOR3 _p, D3DXVECTOR2 _uv) : p(_p), uv(_uv) {}
	/// };
	/// std::vector<temp> tmp;
	/// 
	/// tmp.push_back(temp(D3DXVECTOR3(-1,	1.251, 1), D3DXVECTOR2(0, 0)));
	/// tmp.push_back(temp(D3DXVECTOR3(1,	1.251, 1), D3DXVECTOR2(1, 0)));
	/// tmp.push_back(temp(D3DXVECTOR3(1,	1.251, -1), D3DXVECTOR2(1, 1)));
	/// tmp.push_back(temp(D3DXVECTOR3(-1,	1.251, 1), D3DXVECTOR2(0, 0)));
	/// tmp.push_back(temp(D3DXVECTOR3(1,	1.251, -1), D3DXVECTOR2(1, 1)));
	/// tmp.push_back(temp(D3DXVECTOR3(-1,	1.251, -1), D3DXVECTOR2(0, 1)));
	/// g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &tmp[0], sizeof(temp));
	/// 
	/// 
	/// GetSingleton(TextureRenderer)->SetTransform(m_position, m_bodyLookingRotation, m_scale);
	/// GetSingleton(TextureRenderer)->DrawTexture(m_txWheel);
	/// GetSingleton(TextureRenderer)->DrawTexture(m_txBody);
	/// 
	/// GetSingleton(TextureRenderer)->SetTransform(m_position, m_weaponLookingRotation, m_scale);
	/// GetSingleton(TextureRenderer)->DrawTexture(m_txWeapon);

	return false;
}


bool PlayerTank::ChangeWeapon(const PlayerTankWeaponType & weaponType)
{
	m_playerWeaponType = weaponType;
	m_fireCooltime = 0;
	switch (m_playerWeaponType)
	{
	case PlayerTankWeaponType::Cannon:		m_txWeapon = g_textureContainer->GetTexture(L"Tank_Cannon");		break;
	case PlayerTankWeaponType::AutoCannon:	m_txWeapon = g_textureContainer->GetTexture(L"Tank_AutoCannon");	break;
	case PlayerTankWeaponType::Rocket:		m_txWeapon = g_textureContainer->GetTexture(L"Tank_Rocket");		break;
	}
	return false;
}
