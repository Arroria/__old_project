#include "stdafx.h"
#include "EnemyTank.h"

#include "EnemyTankAdmin.h"


#include "PlayerShell.h"

#define GameDeltaTime (g_processManager->GetDeltaTime())

#define TankMoveSpeed 2 * 10
#define TankTurnSpeed 180

#define WeaponTurnSpeed 120

#define FireCoolTime_Cannon		0.8 * 3
#define FireCoolTime_AutoCannon	0.05 * 2
#define FireCoolTime_Rocket		1.2 * 3

EnemyTank::EnemyTank(const D3DXVECTOR3& position, const EnemyTankType& tankType)
	: BaseTank(position)
	, m_enemyTankAdmin(nullptr)

	, m_txBody(nullptr)
	, m_txWheel(nullptr)
	, m_bodyObjectiveDirection(0, 1, 0)
	, m_bodyLookingDirection(0, -1, 0)
	, m_bodyLookingRotation(0)

	, m_weaponObjectiveDirection(NULL, NULL, NULL)
	, m_weaponLookingDirection(NULL, NULL, NULL)
	, m_weaponLookingRotation(0)

	, m_txWeapon(nullptr)
	, m_playerWeaponType(tankType)
	, m_fireCooltime(0)
{
	m_txWheel = g_textureContainer->GetTexture(L"Tank_Wheel");
	m_txBody = g_textureContainer->GetTexture(L"Tank_Body");

	switch (tankType)
	{
	case EnemyTankType::Cannon:
		m_txWeapon = g_textureContainer->GetTexture(L"Tank_Cannon");
		m_durability = 600;
		m_armour = 60;
		break;
	case EnemyTankType::AutoCannon:
		m_txWeapon = g_textureContainer->GetTexture(L"Tank_AutoCannon");
		m_durability = 300;
		m_armour = 25;
		break;
	case EnemyTankType::Rocket:
		m_txWeapon = g_textureContainer->GetTexture(L"Tank_Rocket");
		m_durability = 400;
		m_armour = 45;
		break;
	}

	//CollisionBox
	float temp = 4;
	m_collisionSize = D3DXVECTOR2(temp, temp);
}
EnemyTank::~EnemyTank()
{
}


void EnemyTank::Move()
{
	//Set body objective rotation;
	D3DXVECTOR3 objectiveDirection(0, 0, 0);
	if (m_needTurn)
	{
		if (rand() % 2) objectiveDirection -= D3DXVECTOR3(0, 1, 0);
		if (rand() % 2) objectiveDirection += D3DXVECTOR3(0, 1, 0);
		if (rand() % 2) objectiveDirection -= D3DXVECTOR3(1, 0, 0);
		if (rand() % 2) objectiveDirection += D3DXVECTOR3(1, 0, 0);
		m_needTurn = false;
	}
	if (objectiveDirection.x != 0 || objectiveDirection.y != 0 || objectiveDirection.z != 0)
		D3DXVec3Normalize(&m_bodyObjectiveDirection, &objectiveDirection);

	if (D3DXVec3Length(&(m_bodyObjectiveDirection - m_bodyLookingDirection)) < 0.001f)
		m_position += m_bodyObjectiveDirection * TankMoveSpeed * GameDeltaTime;
	else
	{
		//Body turn
		D3DXVECTOR3 turnDir;
		D3DXVec3Cross(&turnDir, &m_bodyLookingDirection, &m_bodyObjectiveDirection);

		D3DXMATRIX matTankTurn;
		D3DXMatrixRotationZ(&matTankTurn, D3DXToRadian(TankTurnSpeed * GameDeltaTime) * (turnDir.z < 0 /* True == Clockwise Turn */ ? -1 : 1));
		D3DXVec3TransformCoord(&m_bodyLookingDirection, &m_bodyLookingDirection, &matTankTurn);
		D3DXVec3Normalize(&m_bodyLookingDirection, &m_bodyLookingDirection);
		//Weapon looking direction interaction
			///D3DXVec3TransformCoord(&m_weaponLookingDirection, &m_weaponLookingDirection, &matTankTurn);

		//Turn correction
		D3DXVECTOR3 turnOver;
		D3DXVec3Cross(&turnOver, &m_bodyLookingDirection, &m_bodyObjectiveDirection);
		if (turnOver.z * turnDir.z < 0)
			m_bodyLookingDirection = m_bodyObjectiveDirection;
	}

	//Set body rotation
	m_bodyLookingRotation = D3DXToDegree(acos(D3DXVec3Dot(&D3DXVECTOR3(0, -1, 0), &m_bodyLookingDirection)));
	if (m_bodyLookingDirection.x < 0)
		m_bodyLookingRotation = 360 - m_bodyLookingRotation;
}

void EnemyTank::Aiming()
{
	m_weaponLookingRotation = m_bodyLookingRotation;
	m_weaponLookingDirection = m_bodyLookingDirection;
	//구조 ㅈ됨 어케만들지

	/////Set weapon objective rotation;
	///POINT mousePos = GetSingleton(InputManager)->GetMousePos();
	///m_weaponObjectiveDirection = D3DXVECTOR3(mousePos.x - m_position.x, mousePos.y - m_position.y, 0);
	///D3DXVec3Normalize(&m_weaponObjectiveDirection, &m_weaponObjectiveDirection);

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

void EnemyTank::Attack()
{
	m_fireCooltime -= GameDeltaTime;

	if (m_fireCooltime <= 0)// && (GetSingleton(InputManager)->IsKeyPressed(VK_SPACE) || GetSingleton(InputManager)->IsKeyPressed(VK_LBUTTON)))
	{
		switch (m_playerWeaponType)
		{
		case EnemyTankType::Cannon:
			m_enemyTankAdmin->CreateShell(new PlayerShell(m_position, m_weaponLookingDirection, PlayerShellType::Cannon));
			m_fireCooltime = FireCoolTime_Cannon;
			break;
		case EnemyTankType::AutoCannon:
		{
			m_enemyTankAdmin->CreateShell(new PlayerShell(m_position, m_weaponLookingDirection, PlayerShellType::AutoCannon));
			//shellPos = D3DXVECTOR3(m_weaponLookingDirection.y, -m_weaponLookingDirection.x, 0);
			//m_enemyTankAdmin->CreateShell(new PlayerShell(m_position + shellPos * 10, m_weaponLookingDirection, PlayerShellType::AutoCannon));
			m_fireCooltime = FireCoolTime_AutoCannon;
		}
		break;
		case EnemyTankType::Rocket:
			m_enemyTankAdmin->CreateShell(new PlayerShell(m_position, m_weaponLookingDirection, PlayerShellType::Rocket));
			m_fireCooltime = FireCoolTime_Rocket;
			break;
		}
	}
}

bool EnemyTank::Render()
{
	D3DXMATRIX mat1, mat2, mat3;
	D3DXMatrixTranslation(&mat2, m_position.x, m_position.z + 5, m_position.y);
	D3DXMatrixRotationY(&mat3, -D3DXToRadian(m_bodyLookingRotation - 90));
	D3DXMatrixScaling(&mat1, m_scale.x * 4, m_scale.z * 4, m_scale.y * 4);
	{
		D3DXVECTOR3 scale(m_scale * 0.004f);
		D3DXMatrixScaling(&mat1, scale.x, scale.z, scale.y);
	}
	std::wstring key;
	switch (m_playerWeaponType)
	{
	case EnemyTankType::Cannon:		key = L"Tank_Desert";break;
	case EnemyTankType::AutoCannon: key = L"Tank_Forest";break;
	case EnemyTankType::Rocket:		key = L"Tank_Snow";break;
	}
	g_ase->GetAse(key)->Render(mat1 * mat3 * mat2);
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
	/// tmp.push_back(temp(D3DXVECTOR3(-1, 1.251, 1), D3DXVECTOR2(0, 0)));
	/// tmp.push_back(temp(D3DXVECTOR3(1, 1.251, 1), D3DXVECTOR2(1, 0)));
	/// tmp.push_back(temp(D3DXVECTOR3(1, 1.251, -1), D3DXVECTOR2(1, 1)));
	/// tmp.push_back(temp(D3DXVECTOR3(-1, 1.251, 1), D3DXVECTOR2(0, 0)));
	/// tmp.push_back(temp(D3DXVECTOR3(1, 1.251, -1), D3DXVECTOR2(1, 1)));
	/// tmp.push_back(temp(D3DXVECTOR3(-1, 1.251, -1), D3DXVECTOR2(0, 1)));
	/// g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &tmp[0], sizeof(temp));
	/// 
	/// 
	/// GetSingleton(TextureRenderer)->SetTransform(m_position, m_bodyLookingRotation, m_scale);
	/// GetSingleton(TextureRenderer)->DrawTexture(m_txBody		, D3DXCOLOR(0.5, 0.5, 0.5, 1));
	/// GetSingleton(TextureRenderer)->DrawTexture(m_txWheel	, D3DXCOLOR(0.5, 0.5, 0.5, 1));
	/// GetSingleton(TextureRenderer)->SetTransform(m_position, m_weaponLookingRotation, m_scale);
	/// GetSingleton(TextureRenderer)->DrawTexture(m_txWeapon	, D3DXCOLOR(0.5, 0.5, 0.5, 1));
	return false;
}
