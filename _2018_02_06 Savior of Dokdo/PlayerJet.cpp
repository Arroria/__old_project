#include "stdafx.h"
#include "PlayerJet.h"

#include "Jet.h"
#include "EnemyJet.h"

#include "Bullet.h"


PlayerJet::PlayerJet()
	: m_model(nullptr)
	, m_jet(nullptr)
{
}
PlayerJet::~PlayerJet()
{
}



void PlayerJet::Initialize()
{
	m_model = GetSingleton(ASELoader)->LoadASE(L"./Resource", L"Me_262.ase");

	m_jet = new Jet();
	m_jet->Initialize();
	m_jet->SetPosition(D3DXVECTOR3(0, 15, 0));
}

void PlayerJet::Update(std::vector<Enemy*>* enemyJet, std::vector<Bullet*>* bulletContainer)
{
	ControlAirframe();
	ControlCamera();
	m_lockoner.Lockon(enemyJet);
	Attack(bulletContainer);
}

void PlayerJet::Render()
{
	m_jet->Render(DEVICE, m_model);

	LPD3DXSPRITE sp;
	D3DXCreateSprite(DEVICE, &sp);
	sp->Begin(D3DXSPRITE_ALPHABLEND);
	m_lockoner.RenderLockonEffect(sp);
	sp->End();
	sp->Release();
}

void PlayerJet::Release()
{
	SAFE_RELEASE_DELETE(m_jet);
	SAFE_DELETE(m_model);
}



void PlayerJet::ControlAirframe()
{
	INT rollToRight = 0;
	if (GetSingleton(InputManager)->IsKeyPressed('A'))	rollToRight--;
	if (GetSingleton(InputManager)->IsKeyPressed('D'))	rollToRight++;
	INT accelerate = 0;
	if (GetSingleton(InputManager)->IsKeyPressed('W'))	accelerate++;
	if (GetSingleton(InputManager)->IsKeyPressed('S'))	accelerate--;

	m_jet->Update(accelerate, rollToRight);
}

#define MouseGamdo 5.f
void PlayerJet::ControlCamera()
{
	D3DXVECTOR3 direction; {
		D3DXMATRIX temp;
		D3DXMatrixRotationQuaternion(&temp, &(m_jet->GetYawRotation()));
		D3DXVec3TransformCoord(&direction, &D3DXVECTOR3(0, 0, 1), &temp);
	}
	D3DXVECTOR3& upDir = D3DXVECTOR3(0, 1, 0);
	GetSingleton(CameraManager)->SetCameraPosition(m_jet->GetPosition() - direction * 10 + upDir * 6);
	GetSingleton(CameraManager)->SetCameraAt(m_jet->GetPosition() + direction * 25 - upDir * 5);


	//Mouse smoothing
	constexpr FLOAT CONTRIBUTE = 10;
	POINT mousePos = GetSingleton(InputManager)->GetMousePos();
	D3DXVECTOR2 ratio(mousePos.x / 800.f - 1, -(mousePos.y / 450.f - 1));

	D3DXVECTOR3 cameraInvPitchAxis; {
		D3DXMATRIX temp;
		D3DXMatrixRotationAxis(&temp, &GetSingleton(CameraManager)->GetDataRef().up, D3DXToRadian(-90));
		D3DXVec3TransformNormal(&cameraInvPitchAxis, &GetSingleton(CameraManager)->GetDataRef().direction, &temp);
	}
	D3DXQUATERNION cameraRotXQ, cameraRotYQ;
	D3DXQuaternionRotationAxis(&cameraRotXQ, &cameraInvPitchAxis, D3DXToRadian(ratio.y * CONTRIBUTE));
	D3DXQuaternionRotationAxis(&cameraRotYQ, &D3DXVECTOR3(0, 1, 0), D3DXToRadian(ratio.x * CONTRIBUTE));
	D3DXMATRIX cameraRM;
	D3DXMatrixRotationQuaternion(&cameraRM, &(cameraRotXQ * cameraRotYQ));

	D3DXVECTOR3 cameraNewDir;
	D3DXVec3TransformNormal(&cameraNewDir, &GetSingleton(CameraManager)->GetDataRef().direction, &cameraRM);
	GetSingleton(CameraManager)->SetCameraDirection(cameraNewDir);


	//Camera Shaking
	if (m_cameraShake)
	{
		constexpr FLOAT RATIO = 0.05f;
		D3DXVECTOR3 camDir;
		D3DXVec3Cross(&camDir, &GetSingleton(CameraManager)->GetDataRef().direction, &D3DXVECTOR3(0, 1, 0));
		D3DXMATRIX rot;
		D3DXMatrixRotationAxis(&rot, &GetSingleton(CameraManager)->GetDataRef().direction, D3DXToRadian(rand() % 360));
		D3DXVec3TransformNormal(&camDir, &camDir, &rot);
		GetSingleton(CameraManager)->SetCameraPosition(GetSingleton(CameraManager)->GetDataRef().position + camDir * RATIO * m_cameraShake);
		m_cameraShake--;
	}
}

void PlayerJet::Attack(std::vector<Bullet*>* bulletContainer)
{
	if (m_attackCooltime)
		m_attackCooltime--;
	else
	{
		Enemy* target = m_lockoner.GetTarget();
		if (target && GetSingleton(InputManager)->IsKeyPressed(VK_LBUTTON))
		{
			D3DXVECTOR3 bulletPosition = m_jet->GetPosition();
			Bullet* bullet = new Bullet(bulletPosition, target);
			bulletContainer->push_back(bullet);

			m_attackCooltime = 6;
		}
	}
}



Frame* PlayerJet::GetFrame() { return m_jet; }
