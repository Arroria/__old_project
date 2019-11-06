#include "stdafx.h"
#include "PlayerShip.h"

#include "Ship.h"
#include "Bullet.h"

#include "EnemyJet.h"


PlayerShip::PlayerShip()
	: m_model(nullptr)
	, m_turretModel(nullptr)

	, m_turretRotation(0, 0, 0, 1)
{
}
PlayerShip::~PlayerShip()
{
}



void PlayerShip::Initialize()
{
	m_model = GetSingleton(ASELoader)->LoadASE(L"./Resource", L"Sejong.ASE");
	m_turretModel = GetSingleton(ASELoader)->LoadASE(L"./Resource", L"Sejong_Minigun.ASE");

	m_ship = new Ship();
	m_ship->Initialize();
}

void PlayerShip::Update(std::vector<Enemy*>* enemyJet, std::vector<Bullet*>* bulletContainer)
{
	ControlFrame();
	ControlCamera();
	m_lockoner.Lockon(enemyJet);
	Attack(bulletContainer);
	TurretRotation();
}

void PlayerShip::Render()
{
	m_ship->Render(DEVICE, m_model);

	//Parent
	GeomObj* parentGeomObj = m_model->GetGeomObj(0);
	D3DXMATRIX parentLocalMatrix(parentGeomObj->localMatrix),
		parentInvLocalMatrix;
	D3DXMatrixInverse(&parentInvLocalMatrix, 0, &parentLocalMatrix);

	D3DXVECTOR3 parentPos(m_ship->GetPosition());
	D3DXMATRIX parentR, parentT;
	D3DXMatrixRotationQuaternion(&parentR, &(m_ship->GetRollRotation() * m_ship->GetYawRotation()));
	D3DXMatrixTranslation(&parentT, parentPos.x, parentPos.y, parentPos.z);
	
	D3DXQUATERNION invShipRotation;
	D3DXQuaternionInverse(&invShipRotation, &m_ship->GetYawRotation());
	D3DXMATRIX rm;
	D3DXMatrixRotationQuaternion(&rm, &(m_turretRotation * invShipRotation));


	m_turretModel->SetFVF(DEVICE);
	UINT attributeCount = m_turretModel->GetAttributeCount();
	for (UINT attribute = 0; attribute < attributeCount; attribute++)
	{
		GeomObj* geomObj = m_turretModel->GetGeomObj(attribute);
		GetSingleton(TempShaderHelper)->SetMaterial("g_material", geomObj->mtrlTex->first);
		GetSingleton(TempShaderHelper)->SetTexture("g_texture",   geomObj->mtrlTex->second);
		DEVICE->SetTransform(D3DTS_WORLD, &( geomObj->localMatrixSR * rm * geomObj->localMatrixT * (parentInvLocalMatrix * parentR * parentLocalMatrix) * parentT ));

		UINT passCount = GetSingleton(TempShaderHelper)->Begin(DEVICE);
		for (UINT i = 0; i < passCount; i++)
		{
			GetSingleton(TempShaderHelper)->BeginDraw(i);
			m_turretModel->Render(DEVICE, attribute);
			GetSingleton(TempShaderHelper)->EndDraw();
		}
		GetSingleton(TempShaderHelper)->End();
	}


	LPD3DXSPRITE sp;
	D3DXCreateSprite(DEVICE, &sp);
	sp->Begin(D3DXSPRITE_ALPHABLEND);
	m_lockoner.RenderLockonEffect(sp);
	sp->End();
	sp->Release();
}

void PlayerShip::Release()
{
	SAFE_RELEASE_DELETE(m_ship);
	SAFE_DELETE(m_model);
	SAFE_DELETE(m_turretModel);
}



void PlayerShip::ControlFrame()
{
	INT rollToRight = 0;
	if (GetSingleton(InputManager)->IsKeyPressed('A'))	rollToRight--;
	if (GetSingleton(InputManager)->IsKeyPressed('D'))	rollToRight++;
	INT accelerate = 0;
	if (GetSingleton(InputManager)->IsKeyPressed('W'))	accelerate++;
	if (GetSingleton(InputManager)->IsKeyPressed('S'))	accelerate--;

	m_ship->Update(accelerate, rollToRight);
}

void PlayerShip::ControlCamera()
{
	D3DXVECTOR3 direction;
	{
		D3DXMATRIX temp;
		D3DXMatrixRotationQuaternion(&temp, &(m_ship->GetYawRotation()));
		D3DXVec3TransformCoord(&direction, &D3DXVECTOR3(0, 0, 1), &temp);
	}
	D3DXVECTOR3 upDir(0, 1, 0);
	
	GetSingleton(CameraManager)->SetCameraPosition(m_ship->GetPosition() - direction * 8 + upDir * 1.5);
	GetSingleton(CameraManager)->SetCameraAt(m_ship->GetPosition() + direction * 25 + upDir * 5);



	constexpr FLOAT CONTRIBUTE = 10;
	POINT mousePos = GetSingleton(InputManager)->GetMousePos();
	D3DXVECTOR2 ratio(mousePos.x / 800.f - 1, -(mousePos.y / 450.f - 1));

	D3DXVECTOR3 cameraInvPitchAxis;	{
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

void PlayerShip::Attack(std::vector<Bullet*>* bulletContainer)
{
	m_isFired = false;
	if (m_attackCooltime)
		m_attackCooltime--;
	else if (m_lockoner.GetTarget() && GetSingleton(InputManager)->IsKeyPressed(VK_LBUTTON))
	{
		D3DXVECTOR3& ammoFirePos(m_ammoFirePos); {
			D3DXMATRIX& turretMatrix(m_turretModel->GetGeomObj(0)->localMatrix);
			D3DXVECTOR3 pivot(turretMatrix._41, turretMatrix._42, turretMatrix._43);
			D3DXVECTOR3 offset(D3DXVECTOR3(0.002, 0.643, 0.13) - pivot);

			D3DXMATRIX shipRM;
			D3DXMatrixRotationQuaternion(&shipRM, &m_ship->GetYawRotation());
			
			D3DXVECTOR3 coordPivot;
			D3DXVec3TransformCoord(&coordPivot, &pivot, &shipRM);
			
			D3DXVECTOR3 coordOffset;
			D3DXMATRIX turretRM;
			D3DXMatrixRotationQuaternion(&turretRM, &(m_ship->GetRollRotation() * m_turretRotation));
			D3DXVec3TransformNormal(&coordOffset, &offset, &turretRM);

			ammoFirePos = coordPivot + coordOffset + m_ship->GetPosition();
		}

		Bullet* bullet = new Bullet(ammoFirePos, m_lockoner.GetTarget());
		bulletContainer->push_back(bullet);

		m_isFired = true;
		m_attackCooltime = 6;
	}
}

void PlayerShip::TurretRotation()
{
	D3DXVECTOR3 turretRayOffset; {
		D3DXMATRIX shipRM;
		D3DXMatrixRotationQuaternion(&shipRM, &(m_ship->GetRollRotation() * m_ship->GetYawRotation()));
		D3DXVec3TransformNormal(&turretRayOffset, &D3DXVECTOR3(-0.012, 0.604, -0.435), &shipRM);
		turretRayOffset += m_ship->GetPosition();
	}
	Enemy* lockonTarget = m_lockoner.GetTarget();
	if (lockonTarget)
	{
		D3DXVECTOR3 turretDir;
		D3DXVec3Normalize(&turretDir, &(lockonTarget->GetFrame()->GetPosition() - turretRayOffset));

		D3DXMATRIX turretRotationM;
		D3DXMatrixLookAtLH(&turretRotationM, &D3DXVECTOR3(0, 0, 0), &turretDir, &D3DXVECTOR3(0, 1, 0));
		D3DXMatrixTranspose(&turretRotationM, &turretRotationM);

		D3DXQuaternionRotationMatrix(&m_turretRotation, &turretRotationM);
	}
	else
	{
		D3DXVECTOR3 cameraPosition(GetSingleton(CameraManager)->GetDataRef().position);
		D3DXVECTOR3 cameraDirection(GetSingleton(CameraManager)->GetDataRef().direction);
		D3DXMATRIX cameraRotation; {
			D3DXMatrixLookAtLH(&cameraRotation, &D3DXVECTOR3(0, 0, 0), &cameraDirection, &D3DXVECTOR3(0, 1, 0));
			D3DXMatrixTranspose(&cameraRotation, &cameraRotation);
		}

		//Viewport
		constexpr FLOAT PlaneSize = 10000 * 0.5f;
		constexpr FLOAT RangeOfZ = 1000;
		std::vector<D3DXVECTOR3> tempV;
		tempV.push_back(D3DXVECTOR3(-PlaneSize, +PlaneSize, RangeOfZ));
		tempV.push_back(D3DXVECTOR3(+PlaneSize, +PlaneSize, RangeOfZ));
		tempV.push_back(D3DXVECTOR3(-PlaneSize, -PlaneSize, RangeOfZ));
		tempV.push_back(D3DXVECTOR3(+PlaneSize, -PlaneSize, RangeOfZ));
		for (auto& iter : tempV)
		{
			D3DXVec3TransformCoord(&iter, &iter, &cameraRotation);
			iter += cameraPosition;
		}

		//Ray
		D3DXVECTOR3 rayDirection; {
			D3DXMATRIX vInvM;
			g_processManager->GetDevice()->GetTransform(D3DTS_VIEW, &vInvM);
			D3DXMatrixTranspose(&vInvM, &vInvM);
			{
				POINT mousePos(GetSingleton(InputManager)->GetMousePos());

				//Get Ray At View
				D3DVIEWPORT9 viewPort;
				D3DXMATRIX projectionMatrix;
				DEVICE->GetViewport(&viewPort);
				DEVICE->GetTransform(D3DTS_PROJECTION, &projectionMatrix);

				rayDirection.x = (2.0f *  mousePos.x / viewPort.Width - 1) / projectionMatrix._11;
				rayDirection.y = (-2.0f * mousePos.y / viewPort.Height + 1) / projectionMatrix._22;
				rayDirection.z = 1.0f;
			}
			D3DXVec3TransformNormal(&rayDirection, &rayDirection, &vInvM);
			D3DXVec3Normalize(&rayDirection, &rayDirection);
		}

		//Calculate
		float u, v, distance;
		if (D3DXIntersectTri(&tempV[0], &tempV[1], &tempV[3], &cameraPosition, &rayDirection, &u, &v, &distance) ||
			D3DXIntersectTri(&tempV[0], &tempV[3], &tempV[2], &cameraPosition, &rayDirection, &u, &v, &distance))
		{
			D3DXVECTOR3 pickedPosition(cameraPosition + rayDirection * distance);
			D3DXVECTOR3 turretDirection;
			D3DXVec3Normalize(&turretDirection, &(pickedPosition - turretRayOffset));

			D3DXMATRIX turretRotationM;
			D3DXMatrixLookAtLH(&turretRotationM, &D3DXVECTOR3(0, 0, 0), &turretDirection, &D3DXVECTOR3(0, 1, 0));
			D3DXMatrixTranspose(&turretRotationM, &turretRotationM);
			D3DXQuaternionRotationMatrix(&m_turretRotation, &turretRotationM);
		}
		else
			cout << "viewport error." << endl;
	}
}



Frame* PlayerShip::GetFrame() { return m_ship; }
