#include "stdafx.h"
#include "EnemyShip.h"

#include "Ship.h"
#include "EnemyDropshot.h"


EnemyShip::EnemyShip(ASE * model, const bool& type)
	: m_model(model)
	, m_ship(nullptr)

	, m_rollToRight(false)
	, m_mT(0)
	, m_mD(0)

	, m_attackCooltime(0)
	, m_attackCount(0)

	, m_texHealthFrame(GetSingleton(TextureLoader)->LoadTexture(L"./Resource/ui/EnemyHealthFrame.png", L"EnemyHealthFrame"))
	, m_type(type)
{
}
EnemyShip::~EnemyShip()
{
}



void EnemyShip::Initialize(const D3DXVECTOR3& pos)
{
	m_ship = new Ship();
	m_ship->Initialize();
	m_ship->SetPosition(pos);
}

void EnemyShip::Update(std::vector<EnemyAttack*>* attackContainer, const D3DXVECTOR3 & playerPos)
{
	UINT roll = 0;
	if (m_type)
	{
		if (!m_mD)
		{
			m_mT = rand() % 6;
			m_mD = rand() % 150 + 30;
		}

		switch (m_mT)
		{
		case 0:
			roll = -1;
			break;
		case 1:
			roll = 1;
			break;
		case 2:
			{
				D3DXVECTOR3 objectiveDir(playerPos - m_ship->GetPosition());
				D3DXVec3Normalize(&objectiveDir, &objectiveDir);
				D3DXVECTOR3 myDir; {
					D3DXMATRIX _rm1, _rm2;
					D3DXMatrixRotationQuaternion(&_rm1, &m_ship->GetRollRotation());
					D3DXMatrixRotationQuaternion(&_rm2, &m_ship->GetYawRotation());
					D3DXVec3TransformNormal(&myDir, &D3DXVECTOR3(1, 0, 0), &_rm1);
					D3DXVec3TransformNormal(&myDir, &myDir, &_rm2);
				}
				float dotResult = D3DXVec3Dot(&myDir, &objectiveDir);

				if (dotResult > cos(D3DXToRadian(80)))
					roll = 1;
				else if (dotResult < cos(D3DXToRadian(100)))
					roll = -1;
			}
			break;
		default:
			Attack(attackContainer, playerPos);
			if (m_attackCount > 5)
			{
				m_attackCount = 0;
				m_mT = rand() % 3;
				m_mD = rand() % 150 + 30;
			}
			break;
		}

		m_mD--;
	}
	else
	{
		D3DXVECTOR3 objectiveDir(playerPos - m_ship->GetPosition());
		D3DXVec3Normalize(&objectiveDir, &objectiveDir);
		D3DXVECTOR3 myDir; {
			D3DXMATRIX _rm1, _rm2;
			D3DXMatrixRotationQuaternion(&_rm1, &m_ship->GetRollRotation());
			D3DXMatrixRotationQuaternion(&_rm2, &m_ship->GetYawRotation());
			D3DXVec3TransformNormal(&myDir, &D3DXVECTOR3(1, 0, 0), &_rm1);
			D3DXVec3TransformNormal(&myDir, &myDir, &_rm2);
		}
		float dotResult = D3DXVec3Dot(&myDir, &objectiveDir);

		if (dotResult > 0)
			 roll = 1;
		else
			 roll = -1;
	}
	m_ship->Update(m_ship->GetSpeed() < 0.33 ? 1 : 0, roll);
}

void EnemyShip::Render()
{
	m_ship->Render(DEVICE, m_model);

	//HP
	{
		constexpr FLOAT SCALE = 0.04;


		D3DXVECTOR3 pos(m_ship->GetPosition());

		GetSingleton(TempShaderHelper)->SeleteShader(L"EnemyHealth");
		auto 이름을_뭐라할지_모르겠다_Draw = [&pos, &SCALE](Texture* texture, const float& clip)
		{
			D3DXVECTOR2 texSize(texture->imageInfo.Width, texture->imageInfo.Height);

			GetSingleton(TempShaderHelper)->SetValue("g_health", (LPVOID)&clip, sizeof(float));
			GetSingleton(TempShaderHelper)->SetTexture("g_texture", texture->texture);

			D3DXMATRIX sm, tm, billboardM;
			D3DXMatrixScaling(&sm, texSize.x * SCALE, texSize.y * SCALE, SCALE);
			D3DXMatrixTranslation(&tm, pos.x, pos.y + 1, pos.z);
			DEVICE->GetTransform(D3DTS_VIEW, &billboardM);	billboardM._41 = billboardM._42 = billboardM._43 = 0;
			D3DXMatrixTranspose(&billboardM, &billboardM);

			DEVICE->SetTransform(D3DTS_WORLD, &(sm * billboardM * tm));

			DEVICE->SetRenderState(D3DRS_LIGHTING, false);
			DEVICE->SetFVF(GetSingleton(Plane)->GetFVF());
			UINT passCount = GetSingleton(TempShaderHelper)->Begin(DEVICE);
			for (UINT i = 0; i < passCount; i++)
			{
				GetSingleton(TempShaderHelper)->BeginDraw(i);
				GetSingleton(Plane)->Render(DEVICE);
				GetSingleton(TempShaderHelper)->EndDraw();
			}
			GetSingleton(TempShaderHelper)->End();
		};

		이름을_뭐라할지_모르겠다_Draw(m_texHealthFrame, ((float)m_health / 5));
		GetSingleton(TempShaderHelper)->SeleteShader(L"Light");
	}
}

void EnemyShip::Release()
{
	for (auto& iter : m_myReferencer)
		*iter = nullptr;
	m_myReferencer.clear();
	
	SAFE_RELEASE_DELETE(m_ship);
}



void EnemyShip::Attack(std::vector<EnemyAttack*>* attackContainer, const D3DXVECTOR3 & playerPos)
{
	//if (m_attackCooltime > 10)
	//{
	//	D3DXVECTOR3 myPos(m_ship->GetPosition());
	//	D3DXVECTOR3 myDir(m_ship->GetDirection());
	//
	//	D3DXVECTOR3 attackDir(playerPos - myPos);
	//	D3DXVec3Normalize(&attackDir, &attackDir);
	//
	//	//if (D3DXVec3Dot(&attackDir, &myDir) > cos(D3DXToRadian(30)))
	//	{
	//		EnemyDropshot* eA = new EnemyDropshot(m_ship->GetPosition(), playerPos);
	//		attackContainer->push_back(eA);
	//		m_attackCooltime = 0;
	//	}
	//}
	if (m_attackCooltime > 10)
	{
		EnemyDropshot* eD = new EnemyDropshot(m_ship->GetPosition(), attackContainer);
		attackContainer->push_back(eD);
		m_attackCooltime = 0;
		m_attackCount++;
	}

	m_attackCooltime++;
}



Frame * EnemyShip::GetFrame()	{ return m_ship; }