#include "stdafx.h"
#include "Stage1.h"

#include "Frame.h"

#include "PlayerJet.h"
#include "PlayerShip.h"
#include "PlayerCursor.h"

#include "Boss.h"
#include "Yamato.h"

#include "EnemyJet.h"
#include "EnemyShip.h"
#include "EnemyAttack.h"

#include "Effect3D.h"
#include "DestroyedAirPlane.h"
#include "Explosion.h"
#include "FireScratch.h"

#include "Bullet.h"

Stage1::Stage1()
	: m_player(nullptr)
	, m_cursor(nullptr)

	, m_boss(nullptr)

	, m_skyBox(nullptr)
	, m_seaModel(nullptr)

	, m_font(nullptr)
{
	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
Stage1::~Stage1()
{
}



void Stage1::Initialize()
{
	//CameraSetting
	{
		GetSingleton(CameraManager)->SetProjection(true);
		GetSingleton(CameraManager)->SetCameraPosition(D3DXVECTOR3(0, 0, -1));
		GetSingleton(CameraManager)->SetCameraAt(D3DXVECTOR3(0, 0, 0));
		GetSingleton(CameraManager)->SetCameraUp(D3DXVECTOR3(0, 1, 0));
		GetSingleton(CameraManager)->SetSightAngle(D3DXToRadian(45));
		GetSingleton(CameraManager)->SetAspectRatio(16.f / 9.f);
		GetSingleton(CameraManager)->SetViewRangeNear(0.001f);
		GetSingleton(CameraManager)->SetViewRangeFar(1000);
	}

	m_seaModel = GetSingleton(ASELoader)->LoadASE(L"./Resource", L"Water.ASE");
	m_skyBox = GetSingleton(ASELoader)->LoadASE(L"./Resource", L"Sky.ASE");

	m_aseList.insert(std::make_pair(L"Enemy0", GetSingleton(ASELoader)->LoadASE(L"./Resource/enemy", L"Sinden.ase")));
	m_aseList.insert(std::make_pair(L"Enemy1", GetSingleton(ASELoader)->LoadASE(L"./Resource/enemy", L"A6M1.ase")));
	m_aseList.insert(std::make_pair(L"Enemy2", GetSingleton(ASELoader)->LoadASE(L"./Resource/enemy", L"Japan_Ship_1.ase")));
	m_aseList.insert(std::make_pair(L"Enemy2_1", GetSingleton(ASELoader)->LoadASE(L"./Resource/enemy", L"Japan_Ship_2.ase")));
	m_aseList.insert(std::make_pair(L"Enemy3", GetSingleton(ASELoader)->LoadASE(L"./Resource/enemy", L"Kamikaje.ase")));
	m_aseList.insert(std::make_pair(L"Bullet", GetSingleton(ASELoader)->LoadASE(L"./Resource", L"Bullet.ase")));
	m_aseList.insert(std::make_pair(L"DestroyedEnemy0", GetSingleton(ASELoader)->LoadASE(L"./Resource", L"Dead_Body0.ASE")));
	m_aseList.insert(std::make_pair(L"DestroyedEnemy1", GetSingleton(ASELoader)->LoadASE(L"./Resource", L"Dead_Body1.ASE")));

	GetSingleton(TextureLoader)->LoadTexture(L"./Resource/ui/Ui_Hp.png", L"PlayerUI");
	GetSingleton(TextureLoader)->LoadTexture(L"./Resource/ui/Ui_Health_Bar.png", L"PlayerUI_HealthGauge");

	GetSingleton(TextureLoader)->LoadTexture(L"./Resource/ui/minimap_frame.png", L"Minimap_Frame");
	GetSingleton(TextureLoader)->LoadTexture(L"./Resource/ui/minimap_background.png", L"Minimap_Background");
	GetSingleton(TextureLoader)->LoadTexture(L"./Resource/ui/minimap_enemySpot.png", L"Minimap_EnemySpot");


	GetSingleton(TextureLoader)->LoadTexture(L"./Resource/ui/3d_status_ui.png", L"StatusUI");
	GetSingleton(TextureLoader)->LoadTexture(L"./Resource/ui/3d_health_gauge.png", L"HealthGauge");
	GetSingleton(TextureLoader)->LoadTexture(L"./Resource/ui/3d_item0.png", L"Item0");
	GetSingleton(TextureLoader)->LoadTexture(L"./Resource/ui/3d_item1.png", L"Item1");
	GetSingleton(TextureLoader)->LoadTexture(L"./Resource/ui/3d_item2.png", L"Item2");

	m_player = new PlayerJet();
	//m_player = new PlayerShip();    
	m_player->Initialize();
	
	m_cursor = new PlayerCursor();

	m_boss = new Yamato();
	m_boss->Initialize();


	for (UINT i = 0; i < 25; i++)
		CreateEnemy();

	D3DXCreateSprite(DEVICE, &SingletonSprite);
	D3DXCreateFontW(DEVICE, 100, 0, FW_DONTCARE, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH || FF_DONTCARE, L"", &m_font);

	m_enemyAttack.reserve(3000);
}

void Stage1::Update()
{
	m_player->Update(&m_enemy, &m_bulletContainer);

	if (m_boss)
	{
		m_boss->Update(&m_enemyAttack, m_player->GetFrame()->GetPosition());
		if (m_boss->IsDead())
			SAFE_RELEASE_DELETE(m_boss);
	}

	for (auto& iter = m_enemy.begin(); iter != m_enemy.end();)
	{
		(*iter)->Update(&m_enemyAttack, m_player->GetFrame()->GetPosition());
		if (!(*iter)->GetHealth() || GetSingleton(InputManager)->IsKeyDown('Z'))
		{
			Explosion* explosion = new Explosion((*iter)->GetFrame()->GetPosition());
			m_3dEffect.push_back(explosion);
			for (UINT i = 0; i < 50; i++)
			{
				DestroyedAirPlane* destroyedEnemy = new DestroyedAirPlane(m_aseList[std::wstring(L"DestroyedEnemy") + std::to_wstring(rand() % 2)], (*iter)->GetFrame()->GetPosition());
				m_3dEffect.push_back(destroyedEnemy);

				D3DXCOLOR color;
				switch (rand() % 3)
				{
				case 0: color = D3DXCOLOR(0, 0, 0, 1); break;
				case 1: color = D3DXCOLOR(1, 0, 0, 1); break;
				case 2: color = D3DXCOLOR(0.35, 0.29, 0.2, 1); break;
				}
				FireScratch* fireScratch = new FireScratch((*iter)->GetFrame()->GetPosition(), color);
				m_3dEffect.push_back(fireScratch);
			}
		
			SAFE_RELEASE_DELETE(*iter);
			iter = m_enemy.erase(iter);
		}
		else
			iter++;
	}
	while (m_enemy.size() < 25)
		CreateEnemy();

	for (auto& iter = m_bulletContainer.begin(); iter != m_bulletContainer.end();)
	{
		(*iter)->Update();
		if ((*iter)->IsExpired())
		{
			///SAFE_RELEASE_DELETE(*iter);
			SAFE_DELETE(*iter);
			iter = m_bulletContainer.erase(iter);
		}
		else
			iter++;
	}

	//for (UINT i = iterP; iter != m_enemyAttack.end();)
	for (auto& iter = m_enemyAttack.begin(); iter != m_enemyAttack.end();)
	{
		(*iter)->Update(m_player);
		if ((*iter)->IsExpired())
		{
			///SAFE_RELEASE_DELETE(*iter);
			SAFE_DELETE(*iter);
			iter = m_enemyAttack.erase(iter);
		}
		else
			iter++;
	}

	for (auto& iter = m_3dEffect.begin(); iter != m_3dEffect.end();)
	{
		(*iter)->Update();
		if ((*iter)->IsExpired())
		{
			///SAFE_RELEASE_DELETE(*iter);
			SAFE_DELETE(*iter);
			iter = m_3dEffect.erase(iter);
		}
		else
			iter++;
	}
}

void Stage1::Render()
{
	//SkyBox
	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	{
		CONST FLOAT SKYBOX_SIZE = GetSingleton(CameraManager)->GetData().viewFarRange / 1.7f;

		D3DXMATRIX skyBoxSM, playerTM;
		D3DXVECTOR3& playerPos = m_player->GetFrame()->GetPosition();
		D3DXMatrixScaling(&skyBoxSM, SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE);
		D3DXMatrixTranslation(&playerTM, playerPos.x, playerPos.y, playerPos.z);

		m_skyBox->SetFVF(DEVICE);
		GeomObj* geomObj = m_skyBox->GetGeomObj(0);
		DEVICE->SetMaterial(&geomObj->mtrlTex->first);
		DEVICE->SetTexture(0, geomObj->mtrlTex->second);
		HRESULT hr = DEVICE->SetTransform(D3DTS_WORLD, &(geomObj->localMatrixSR * skyBoxSM * geomObj->localMatrixT * playerTM));

		DEVICE->SetRenderState(D3DRS_LIGHTING, false);
		m_skyBox->Render(DEVICE, 0);
	}

	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	//Sea
	{
		CONST INT SEA_TELEPORT_DISTANCE = 10;

		D3DXMATRIX seaTM;
		D3DXVECTOR3 playerPos = m_player->GetFrame()->GetPosition();
		playerPos.x = (int)(playerPos.x / SEA_TELEPORT_DISTANCE) * SEA_TELEPORT_DISTANCE;
		playerPos.z = (int)(playerPos.z / SEA_TELEPORT_DISTANCE) * SEA_TELEPORT_DISTANCE;

		D3DXMatrixTranslation(&seaTM, playerPos.x, 0, playerPos.z);

		m_seaModel->SetFVF(DEVICE);
		GetSingleton(TempShaderHelper)->SeleteShader(L"Diffuse");
		UINT attributeCount = m_seaModel->GetAttributeCount();
		for (UINT attribute = 0; attribute < attributeCount; attribute++)
		{
			GeomObj* obj = m_seaModel->GetGeomObj(attribute);
			GetSingleton(TempShaderHelper)->SetMaterial("g_material", obj->mtrlTex->first);
			GetSingleton(TempShaderHelper)->SetTexture("g_texture", obj->mtrlTex->second);
			DEVICE->SetTransform(D3DTS_WORLD, &(obj->localMatrix * seaTM));

			UINT passCount = GetSingleton(TempShaderHelper)->Begin(DEVICE);
			for (UINT i = 0; i < passCount; i++)
			{
				GetSingleton(TempShaderHelper)->BeginDraw(i);
				m_seaModel->Render(DEVICE, attribute);
				GetSingleton(TempShaderHelper)->EndDraw();
			}
			GetSingleton(TempShaderHelper)->End();
		}
	}
	GetSingleton(TempShaderHelper)->SeleteShader(L"Light");

	m_player->Render();

	if (m_boss)
		m_boss->Render();
	
	for (auto& iter : m_enemy)
		iter->Render();

	for (auto& iter : m_3dEffect)
		iter->Render();

	for (auto& iter : m_bulletContainer)
		iter->Render(m_aseList[L"Bullet"]);
	for (auto& iter : m_enemyAttack)
		iter->Render();


	//Draw 3D UI
	{
		constexpr FLOAT SCALE = 0.0115;
		constexpr FLOAT UI_DISTANCE = 5;
		constexpr FLOAT DIR_ROT_DEGREE = 40;
		constexpr FLOAT VIEW_ROT_DEGREE = 25;

		D3DXVECTOR3& playerPos(m_player->GetFrame()->GetPosition());
		D3DXVECTOR3& playerDir(m_player->GetFrame()->GetDirection());
		D3DXMATRIX playerRYM;	D3DXMatrixRotationQuaternion(&playerRYM, &(m_player->GetFrame()->GetYawRotation()));

		//Status UI
		{
			DEVICE->SetRenderState(D3DRS_ZENABLE, false);
			Texture* statusUI = GetSingleton(TextureLoader)->GetTexture(L"StatusUI");
			Texture* healthGauge = GetSingleton(TextureLoader)->GetTexture(L"HealthGauge");


			D3DXVECTOR3 myOffSetPos; {
				D3DXMATRIX _rym;
				D3DXMatrixRotationY(&_rym, D3DXToRadian(-DIR_ROT_DEGREE));
				D3DXVec3TransformNormal(&myOffSetPos, &D3DXVECTOR3(0, 0, 1), &(_rym * playerRYM));
			}

			D3DXMATRIX sm, rm, worldPosTM, localPosTM;
			D3DXMatrixScaling(&sm, SCALE * statusUI->imageInfo.Width, SCALE * statusUI->imageInfo.Height, SCALE);
			/*rm*/ {
				D3DXMATRIX _rym;
				D3DXMatrixRotationY(&_rym, D3DXToRadian(-VIEW_ROT_DEGREE));
				rm = _rym * playerRYM;
			}
			D3DXMatrixTranslation(&localPosTM, myOffSetPos.x * UI_DISTANCE, myOffSetPos.y * UI_DISTANCE, myOffSetPos.z * UI_DISTANCE);
			D3DXMatrixTranslation(&worldPosTM, playerPos.x, playerPos.y, playerPos.z);

			D3DMATERIAL9 whiteMtrl; {
				ZeroMemory(&whiteMtrl, sizeof(whiteMtrl));
				whiteMtrl.Ambient = whiteMtrl.Diffuse = whiteMtrl.Specular = D3DXCOLOR(1, 1, 1, 1);
			}
			DEVICE->SetMaterial(&whiteMtrl);
			DEVICE->SetRenderState(D3DRS_LIGHTING, false);
			DEVICE->SetFVF(GetSingleton(Plane)->GetFVF());

			constexpr FLOAT GAUGE_DISTANCE = 21;

			//Gauge
			DEVICE->SetTexture(0, healthGauge->texture);
			for (UINT i = 0; i < (m_player->GetHealth() + 9) / 10; i++)
			{
				D3DXVECTOR3 offset; {
					D3DXMATRIX temp;
					D3DXMatrixRotationY(&temp, D3DXToRadian(90));

					D3DXVec3TransformNormal(&offset, &D3DXVECTOR3(0, 0, 1), &(rm * temp));
					offset *= SCALE * (i * GAUGE_DISTANCE - 70);
					/*Y offset*/ {
						D3DXVECTOR3 yawOffset;
						D3DXMATRIX _rm;
						D3DXMatrixLookAtLH(&_rm, &D3DXVECTOR3(0, 0, 0), &offset, &D3DXVECTOR3(0, 1, 0));
						D3DXMatrixTranspose(&_rm, &_rm);
						D3DXVec3TransformNormal(&yawOffset, &D3DXVECTOR3(0, 1, 0), &_rm);

						offset -= yawOffset * SCALE * 20;
					}
				}

				D3DXMATRIX sm, tm;
				D3DXMatrixScaling(&sm, SCALE * healthGauge->imageInfo.Width, SCALE * healthGauge->imageInfo.Height, SCALE);
				D3DXMatrixTranslation(&tm, offset.x, offset.y, offset.z);

				DEVICE->SetTransform(D3DTS_WORLD, &(sm * rm * localPosTM * worldPosTM * tm));
				GetSingleton(Plane)->Render(DEVICE);
			}

			//Font
			{
				D3DXMATRIX identity;
				D3DXMatrixIdentity(&identity);
				DEVICE->SetTransform(D3DTS_WORLD, &identity);
				DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
				SingletonSprite->Begin(D3DXSPRITE_DONOTMODIFY_RENDERSTATE | D3DXSPRITE_DO_NOT_ADDREF_TEXTURE | D3DXSPRITE_OBJECTSPACE);

				constexpr FLOAT FONT_SCALE = 0.004;

				RECT zeroRect; ZeroMemory(&zeroRect, sizeof(RECT));
				D3DXVECTOR3 pitchOffset; {
					D3DXMATRIX temp;
					D3DXMatrixRotationY(&temp, D3DXToRadian(90));

					D3DXVec3TransformNormal(&pitchOffset, &D3DXVECTOR3(0, 0, 1), &(rm * temp));
					pitchOffset *= SCALE;
				}
				D3DXVECTOR3 yawOffset; {
					D3DXMATRIX _rm;
					D3DXMatrixLookAtLH(&_rm, &D3DXVECTOR3(0, 0, 0), &pitchOffset, &D3DXVECTOR3(0, 1, 0));
					D3DXMatrixTranspose(&_rm, &_rm);
					D3DXVec3TransformNormal(&yawOffset, &D3DXVECTOR3(0, 1, 0), &_rm);
					yawOffset *= SCALE;
				}
				D3DXMATRIX fontLocalRotation, sm, tm;
				D3DXMatrixRotationX(&fontLocalRotation, D3DXToRadian(180));

				auto DrawTxt = [&zeroRect, &pitchOffset, &yawOffset, &fontLocalRotation, &rm, &localPosTM, &worldPosTM](LPD3DXFONT font, LPD3DXSPRITE sprite, const std::wstring& text, const float& scale, const float& x, const float& y)
				{
					D3DXMATRIX sm, tm;
					D3DXMatrixScaling(&sm, scale, scale, scale);
					D3DXMatrixTranslation(&tm,
						pitchOffset.x * x + yawOffset.x * y,
						pitchOffset.y * x + yawOffset.y * y,
						pitchOffset.z * x + yawOffset.z * y);

					sprite->SetTransform(&(fontLocalRotation * sm * rm * localPosTM * worldPosTM * tm));
					font->DrawTextW(sprite, text.data(), -1, &zeroRect, DT_NOCLIP | DT_LEFT | DT_BOTTOM, D3DXCOLOR(1, 1, 1, 1));
				};
				DrawTxt(m_font, SingletonSprite, L"∞", FONT_SCALE, -100, 10);
				DrawTxt(m_font, SingletonSprite, L"0", FONT_SCALE, -10, 10);
				DrawTxt(m_font, SingletonSprite, L"0", FONT_SCALE, 75, 10);
				SingletonSprite->End();
				DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			}

			//Frame
			DEVICE->SetFVF(GetSingleton(Plane)->GetFVF());
			DEVICE->SetTexture(0, statusUI->texture);
			DEVICE->SetTransform(D3DTS_WORLD, &(sm * rm * localPosTM * worldPosTM));
			GetSingleton(Plane)->Render(DEVICE);
			DEVICE->SetRenderState(D3DRS_ZENABLE, true);
		}

		//Item UI
		{
			DEVICE->SetRenderState(D3DRS_ZENABLE, false);
			Texture* item0 = GetSingleton(TextureLoader)->GetTexture(L"Item0");
			Texture* item1 = GetSingleton(TextureLoader)->GetTexture(L"Item1");
			Texture* item2 = GetSingleton(TextureLoader)->GetTexture(L"Item2");


			D3DXVECTOR3 myOffSetPos; {
				D3DXMATRIX _rym;
				D3DXMatrixRotationY(&_rym, D3DXToRadian(DIR_ROT_DEGREE));
				D3DXVec3TransformNormal(&myOffSetPos, &D3DXVECTOR3(0, 0, 1), &(_rym * playerRYM));
			}

			D3DXMATRIX sm, rm, worldPosTM, localPosTM;
			D3DXMatrixScaling(&sm, SCALE * item0->imageInfo.Width, SCALE * item0->imageInfo.Height, SCALE);
			/*rm*/ {
				D3DXMATRIX _rym;
				D3DXMatrixRotationY(&_rym, D3DXToRadian(VIEW_ROT_DEGREE));
				rm = _rym * playerRYM;
			}
			D3DXMatrixTranslation(&localPosTM, myOffSetPos.x * UI_DISTANCE, myOffSetPos.y * UI_DISTANCE, myOffSetPos.z * UI_DISTANCE);
			D3DXMatrixTranslation(&worldPosTM, playerPos.x, playerPos.y, playerPos.z);

			D3DMATERIAL9 whiteMtrl; {
				ZeroMemory(&whiteMtrl, sizeof(whiteMtrl));
				whiteMtrl.Ambient = whiteMtrl.Diffuse = whiteMtrl.Specular = D3DXCOLOR(1, 1, 1, 1);
			}
			DEVICE->SetMaterial(&whiteMtrl);
			DEVICE->SetRenderState(D3DRS_LIGHTING, false);
			DEVICE->SetFVF(GetSingleton(Plane)->GetFVF());


			D3DXVECTOR3 offset; {
				D3DXMATRIX temp;
				D3DXMatrixRotationY(&temp, D3DXToRadian(90));

				D3DXVec3TransformNormal(&offset, &D3DXVECTOR3(0, 0, 1), &(rm * temp));
				offset *= SCALE;
			}

			auto RenderItemUI = [&offset, &sm, &rm, &localPosTM, &worldPosTM](LPDIRECT3DDEVICE9 device, Texture* texture, const float& pos)
			{
				D3DXMATRIX tm;
				D3DXMatrixTranslation(&tm, offset.x * pos, offset.y * pos, offset.z * pos);

				device->SetTexture(0, texture->texture);
				device->SetTransform(D3DTS_WORLD, &(sm * rm * localPosTM * worldPosTM * tm));
				GetSingleton(Plane)->Render(device);
			};

			RenderItemUI(DEVICE, item0, -70);
			RenderItemUI(DEVICE, item1, 0);
			RenderItemUI(DEVICE, item2, 70);
			DEVICE->SetRenderState(D3DRS_ZENABLE, true);
		}

		//2D TEMP CODE
		{
		///Texture* statusUI = GetSingleton(TextureLoader)->GetTexture(L"StatusUI");
		///Texture* healthGauge = GetSingleton(TextureLoader)->GetTexture(L"HealthGauge");
		///Texture* item0 = GetSingleton(TextureLoader)->GetTexture(L"Item0");
		///Texture* item1 = GetSingleton(TextureLoader)->GetTexture(L"Item1");
		///
		///sp->Draw(statusUI->texture, nullptr, &D3DXVECTOR3(0, statusUI->imageInfo.Height, 0), &D3DXVECTOR3(0, 900, 0), D3DXCOLOR(1, 1, 1, 1));
		///for (UINT i = 0; i < 10; i++)
		///{
		///	sp->Draw(healthGauge->texture, nullptr, &D3DXVECTOR3(0, healthGauge->imageInfo.Height, 0), &D3DXVECTOR3(60 + i * 20 , 900 - 10, 0), D3DXCOLOR(1, 1, 1, 1));
		///}
		}
		
	}



	//플레이어 격발 이펙트
	if (m_player->IsFired())
	{
		D3DXMATRIX sm, rm, tm;
		D3DXMatrixScaling(&sm, 0.5f, 0.5f, 0.5f);

		D3DXMATRIX viewM;
		DEVICE->GetTransform(D3DTS_VIEW, &viewM);
		viewM._41 = viewM._42 = viewM._43 = 0;
		D3DXMatrixTranspose(&rm, &viewM);

		D3DXVECTOR3& ammoFiredPosition = m_player->GetAmmoFirePosition();
		D3DXMatrixTranslation(&tm, ammoFiredPosition.x, ammoFiredPosition.y, ammoFiredPosition.z + 0.25);
		
		ASE* ase = GetSingleton(ASELoader)->LoadASE(L"./Resource/effect", L"fire.ASE");

		DEVICE->SetRenderState(D3DRS_LIGHTING, false);
		ase->SetFVF(DEVICE);
		UINT attributeCount = ase->GetAttributeCount();
		for (UINT attribute = 0; attribute < attributeCount; attribute++)
		{
			GeomObj* geomObj = ase->GetGeomObj(attribute);
			DEVICE->SetMaterial(&geomObj->mtrlTex->first);
			DEVICE->SetTexture(0, geomObj->mtrlTex->second);
			DEVICE->SetTransform(D3DTS_WORLD, &(geomObj->localMatrixSR * (sm * rm) * geomObj->localMatrixT * tm));
			ase->Render(DEVICE, attribute);
		}
		DEVICE->SetRenderState(D3DRS_LIGHTING, true);

		SAFE_DELETE(ase);
	}



	//Draw 2D UI
	{
		SingletonSprite->Begin(D3DXSPRITE_ALPHABLEND);
		m_cursor->Render(SingletonSprite, m_player->GetLockoner()->GetTarget());

		//Minimap
		{
			static FLOAT UI_SIZE = 1;
			if (GetSingleton(InputManager)->IsKeyPressed('Q'))
				UI_SIZE -= 0.05f;
			if (GetSingleton(InputManager)->IsKeyPressed('E'))
				UI_SIZE += 0.05f;

			CONST FLOAT MINIMAP_MAGNIFICATION = 4;
			CONST FLOAT MINIMAP_RANGE = MINIMAP_MAGNIFICATION * 125;

			Texture* frameTex = GetSingleton(TextureLoader)->GetTexture(L"Minimap_Frame");
			Texture* backgroundTex = GetSingleton(TextureLoader)->GetTexture(L"Minimap_Background");
			Texture* enemyPointTex = GetSingleton(TextureLoader)->GetTexture(L"Minimap_EnemySpot");

			D3DXMATRIX minimapWorldMatrix, mSM, mTM;
			D3DXMatrixScaling(&mSM, UI_SIZE, UI_SIZE, 0);
			D3DXMatrixTranslation(&mTM, 1600 - backgroundTex->imageInfo.Width * UI_SIZE, 900 - backgroundTex->imageInfo.Height * UI_SIZE, 0);
			minimapWorldMatrix = mSM * mTM;

			SingletonSprite->SetTransform(&minimapWorldMatrix);
			SingletonSprite->Draw(backgroundTex->texture, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, 1));

			D3DXVECTOR3 playerPos(m_player->GetFrame()->GetPosition());	playerPos.y = 0;
			D3DXMATRIX playerInvRotationM; {
				D3DXQUATERNION playerRotation;
				D3DXQuaternionInverse(&playerRotation, &m_player->GetFrame()->GetYawRotation());
				D3DXMatrixRotationQuaternion(&playerInvRotationM, &playerRotation);
			}
			for (auto& iter : m_enemy)
			{
				D3DXVECTOR3 targetPosition(iter->GetFrame()->GetPosition());	targetPosition.y = 0;
				D3DXVECTOR3 positionForPlayer(targetPosition - playerPos);

				float		distance = D3DXVec3Length(&positionForPlayer);
				D3DXVECTOR3 direction;
				D3DXVec3Normalize(&direction, &positionForPlayer);

				D3DXVECTOR3 positionForMinimap(direction * (distance > MINIMAP_RANGE ? MINIMAP_RANGE : distance) / MINIMAP_MAGNIFICATION);
				D3DXVec3TransformCoord(&positionForMinimap, &positionForMinimap, &playerInvRotationM);
				positionForMinimap.z *= -1;

				D3DXMATRIX pM;
				D3DXMatrixTranslation(&pM,
					1 + ((float)frameTex->imageInfo.Width - enemyPointTex->imageInfo.Width)   * 0.5f + positionForMinimap.x,
					1 + ((float)frameTex->imageInfo.Height - enemyPointTex->imageInfo.Height) * 0.5f + positionForMinimap.z,
					0);
				//+ positionForMinimap.x
				//+ positionForMinimap.z
				///positionForMinimap.x +  frameTex->imageInfo.Width * UI_SIZE * 0.5f - enemyPointTex->imageInfo.Width * 0.5f	 + 1,
				///-positionForMinimap.z + frameTex->imageInfo.Height * UI_SIZE * 0.5f - enemyPointTex->imageInfo.Height * 0.5f + 1,
				///0);
				SingletonSprite->SetTransform(&(pM * minimapWorldMatrix));
				SingletonSprite->Draw(enemyPointTex->texture, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, 1));
			}

			SingletonSprite->SetTransform(&minimapWorldMatrix);
			SingletonSprite->Draw(frameTex->texture, nullptr, nullptr, nullptr, D3DXCOLOR(1, 1, 1, 1));
		}



		SingletonSprite->End();
	}
}

void Stage1::Release()
{
	SAFE_RELEASE_DELETE(m_player);
	SAFE_RELEASE_DELETE(m_boss);

	for (auto& iter : m_enemy)
		SAFE_DELETE(iter);
	m_enemy.clear();


	for (auto& iter : m_bulletContainer)
		SAFE_DELETE(iter);
	m_bulletContainer.clear();

	for (auto& iter : m_enemyAttack)
		SAFE_DELETE(iter);
	m_enemyAttack.clear();


	for (auto& iter : m_aseList)
		SAFE_DELETE(iter.second);
	m_aseList.clear();

	SingletonSprite->Release();
}



void Stage1::CreateEnemy()
{
	constexpr FLOAT DISTANCE = 250;
	D3DXVECTOR3 enemyPos; {
		D3DXMATRIX dir; {
			D3DXMATRIX randomRot, cameraRot;
			
			float sightAngle = GetSingleton(CameraManager)->GetDataRef().sightAngle;
			D3DXMatrixRotationY(&randomRot, sightAngle + D3DXToRadian( rand() % (int)(360 - D3DXToDegree(sightAngle * 2)) ));

			D3DXVECTOR3 cameraDir(GetSingleton(CameraManager)->GetDataRef().direction);
			cameraDir.y = 0;
			D3DXVec3Normalize(&cameraDir, &cameraDir);
			D3DXMatrixLookAtLH(&cameraRot, &D3DXVECTOR3(0, 0, 0), &cameraDir, &D3DXVECTOR3(0, 1, 0));
			D3DXMatrixTranspose(&cameraRot, &cameraRot);

			dir = randomRot * cameraRot;
		}
		D3DXVec3TransformNormal(&enemyPos, &D3DXVECTOR3(0, 0, DISTANCE), &dir);

		enemyPos += GetSingleton(CameraManager)->GetDataRef().position;
		enemyPos.y = 0;
	}

	if (rand() & 0b1)
	{
		EnemyJet* enemyJet = new EnemyJet(m_aseList[rand() % 2 ? L"Enemy0" : L"Enemy1"]);
		enemyJet->Initialize(D3DXVECTOR3(enemyPos.x, 10 + rand() % 50 * 0.1f, enemyPos.z));
		enemyJet->SetHitEffecter(m_cursor);
		m_enemy.push_back(enemyJet);
	}
	else
	{
		bool kkj = rand() & 0b1;
		EnemyShip* enemyShip = new EnemyShip(m_aseList[kkj ? L"Enemy2" : L"Enemy3"], kkj);
		enemyShip->Initialize(enemyPos);
		enemyShip->SetHitEffecter(m_cursor);
		m_enemy.push_back(enemyShip);
	}
}
