#include "stdafx.h"
#include "EnemyJet.h"

#include "Jet.h"
#include "Player.h"

#include "EnemyCannon.h"

EnemyJet::EnemyJet(ASE* model)
	: m_model(model)
	, m_jet(nullptr)

	, m_pattern(0)
	, m_patternDuration(0)
	, m_rollToRight(false)

	, m_attackCooltime(0)

	, m_texHealthFrame(m_texHealthFrame = GetSingleton(TextureLoader)->LoadTexture(L"./Resource/ui/EnemyHealthFrame.png", L"EnemyHealthFrame"))
{
}
EnemyJet::~EnemyJet()
{
}



void EnemyJet::Initialize(const D3DXVECTOR3& pos)
{
	m_jet = new Jet();
	m_jet->Initialize();
	m_jet->SetPosition(pos);
}

void EnemyJet::Update(std::vector<EnemyAttack*>* attackContainer, const D3DXVECTOR3& playerPos)
{
	if (!m_patternDuration)
	{
		m_pattern = rand() % 3;
		m_patternDuration = rand() % 240 + 60;

		switch (m_pattern)
		{
		case 0:	m_rollToRight = (rand() % 2 - 0.5f) * 2;	break;
		case 1:	m_rollToRight = 0;							break;
		}
	}
	m_patternDuration--;

	switch (m_pattern)
	{
	case 0:	AIPattern_Roll(playerPos);			break;
	case 1:	AIPattern_GoForward(playerPos);		break;
	case 2:	AIPattern_FollowPlayer(playerPos);	break;
	}
	m_jet->Update(0, m_rollToRight);

	Attack(attackContainer, playerPos);
}

void EnemyJet::Render()
{
	m_jet->Render(DEVICE, m_model);

	//HP
	{
		constexpr FLOAT SCALE = 0.04;


		D3DXVECTOR3 pos(m_jet->GetPosition());

		GetSingleton(TempShaderHelper)->SeleteShader(L"EnemyHealth");
		auto 이름을_뭐라할지_모르겠다_Draw = [&pos, &SCALE](Texture* texture, const float& clip)
		{
			while (!texture)
				texture = GetSingleton(TextureLoader)->LoadTexture(L"./Resource/ui/EnemyHealthFrame.png", L"EnemyHealthFrame");


			D3DXVECTOR2 texSize(texture->imageInfo.Width, texture->imageInfo.Height);

			GetSingleton(TempShaderHelper)->SetValue("g_health", (LPVOID)&clip, sizeof(float));
			GetSingleton(TempShaderHelper)->SetTexture("g_texture", texture->texture);

			D3DXMATRIX sm, tm, billboardM;
			D3DXMatrixScaling(&sm, texSize.x * SCALE, texSize.y * SCALE, SCALE);
			D3DXMatrixTranslation(&tm, pos.x, pos.y - 1, pos.z);
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

void EnemyJet::Release()
{
	for (auto& iter : m_myReferencer)
		*iter = nullptr;
	m_myReferencer.clear();

	SAFE_RELEASE_DELETE(m_jet);
}



void EnemyJet::AIPattern_Roll(const D3DXVECTOR3& playerPos)
{
	if (D3DXVec3Length(&(playerPos - m_jet->GetPosition())) > 50)
	{
		m_pattern = 2;
		m_patternDuration = 180;
	}
}

void EnemyJet::AIPattern_GoForward(const D3DXVECTOR3& playerPos)
{
	if (D3DXVec3Length(&(playerPos - m_jet->GetPosition())) > 500)
	{
		m_pattern = 2;
		m_patternDuration = 180;
	}
}

void EnemyJet::AIPattern_FollowPlayer(const D3DXVECTOR3& playerPos)
{
	D3DXVECTOR3 myPosition(m_jet->GetPosition());
	D3DXVECTOR3 myPitchAxis; {
		D3DXMATRIX r;
		D3DXMatrixRotationQuaternion(&r, &m_jet->GetYawRotation());
		D3DXVec3TransformCoord(&myPitchAxis, &D3DXVECTOR3(1, 0, 0), &r);
	}

	D3DXVECTOR3 direction(playerPos - myPosition);

	float cosTh = D3DXVec3Dot(&direction, &myPitchAxis);
	m_rollToRight =
		(D3DXVec3Length(&direction) <  10 && fabs(cosTh) < D3DXToRadian(30.f) ?
			0 :
			(cosTh > 0 ? 1 : -1));
}



void EnemyJet::Attack(std::vector<EnemyAttack*>* attackContainer, const D3DXVECTOR3& playerPos)
{
	if (m_attackCooltime > 7)
	{
		D3DXVECTOR3 myPos(m_jet->GetPosition());
		D3DXVECTOR3 myDir(m_jet->GetDirection());

		D3DXVECTOR3 attackDir(playerPos - myPos);
		D3DXVec3Normalize(&attackDir, &attackDir);

		if (D3DXVec3Dot(&attackDir, &myDir) > cos(D3DXToRadian(30)))
		{
			D3DXVECTOR3 attackYawDir; {
				D3DXVec3Cross(&attackYawDir, &attackDir, &D3DXVECTOR3(0, 1, 0));
			}
			D3DXMATRIX rot, dir;
			D3DXMatrixRotationAxis(&rot, &attackYawDir, D3DXToRadian(rand() % 100 * 0.05f));
			D3DXMatrixRotationAxis(&dir, &attackDir, D3DXToRadian(rand() % 360));
			D3DXVec3TransformNormal(&attackDir, &attackDir, &rot);
			D3DXVec3TransformNormal(&attackDir, &attackDir, &dir);


			EnemyCannon* eA = new EnemyCannon(m_jet->GetPosition(), attackDir);
			attackContainer->push_back(eA);
			m_attackCooltime = 0;
		}
	}

	m_attackCooltime++;
}


Frame* EnemyJet::GetFrame() { return m_jet; }
