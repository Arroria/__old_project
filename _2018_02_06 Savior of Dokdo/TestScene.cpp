#include "stdafx.h"
#include "TestScene.h"


TestScene::TestScene()
	: m_model1(nullptr)
	, m_model2(nullptr)

	, m_cameraPos(0, 0, -1)
	, m_cameraRotationForX(0)
	, m_cameraRotationForY(0)

	, m_pos(0, 0, 0)
	, m_rot(0, 0)

	, m_font(nullptr)
{
}
TestScene::~TestScene()
{
}


void TestScene::Initialize()
{
	GetSingleton(CameraManager)->SetProjection(true);
	GetSingleton(CameraManager)->SetCameraPosition(D3DXVECTOR3(0, 0, -1));
	GetSingleton(CameraManager)->SetCameraAt(D3DXVECTOR3(0, 0, 0));
	GetSingleton(CameraManager)->SetCameraUp(D3DXVECTOR3(0, 1, 0));
	GetSingleton(CameraManager)->SetSightAngle(D3DXToRadian(45));
	GetSingleton(CameraManager)->SetAspectRatio(16.f / 9.f);
	GetSingleton(CameraManager)->SetViewRangeNear(0.001f);
	GetSingleton(CameraManager)->SetViewRangeFar(1000);

	m_model1 = GetSingleton(ASELoader)->LoadASE(L"./Resource", L"TEST2.ASE");
	m_model2 = GetSingleton(ASELoader)->LoadASE(L"./Resource", L"Dead_Body1.ASE");

	D3DXCreateFontW(DEVICE, 100, 0, FW_DONTCARE, 1, false, DEFAULT_CHARSET, NULL, NULL, DEFAULT_PITCH | FF_DONTCARE, L"", &m_font);
}

void TestScene::Update()
{
	//Camera
	{
		CONST FLOAT CAMERA_MOVE_SPEED = 0.2f;
		CONST FLOAT CAMERA_ROTATION_SPEED = 0.2f;
		
		if (GetSingleton(InputManager)->IsKeyPressed('W'))			m_cameraPos += GetSingleton(CameraManager)->GetDataRef().direction * CAMERA_MOVE_SPEED;
		if (GetSingleton(InputManager)->IsKeyPressed('S'))			m_cameraPos -= GetSingleton(CameraManager)->GetDataRef().direction * CAMERA_MOVE_SPEED;
		if (GetSingleton(InputManager)->IsKeyPressed('A'))			{ D3DXVECTOR3 leftDir;	D3DXVec3Cross(&leftDir	, &GetSingleton(CameraManager)->GetDataRef().direction, &GetSingleton(CameraManager)->GetDataRef().up);	m_cameraPos += leftDir  * CAMERA_MOVE_SPEED; }
		if (GetSingleton(InputManager)->IsKeyPressed('D'))			{ D3DXVECTOR3 rightDir;	D3DXVec3Cross(&rightDir	, &GetSingleton(CameraManager)->GetDataRef().up, &GetSingleton(CameraManager)->GetDataRef().direction);	m_cameraPos += rightDir * CAMERA_MOVE_SPEED; }
		if (GetSingleton(InputManager)->IsKeyPressed(VK_SPACE))		m_cameraPos.y += CAMERA_MOVE_SPEED;
		if (GetSingleton(InputManager)->IsKeyPressed(VK_LSHIFT))	m_cameraPos.y -= CAMERA_MOVE_SPEED;
		
		if (GetSingleton(InputManager)->IsKeyNone(VK_MENU))	m_cameraRotationForY += GetSingleton(InputManager)->GetMouseDelta().x * CAMERA_ROTATION_SPEED;
		if (GetSingleton(InputManager)->IsKeyNone(VK_MENU))
		{
			m_cameraRotationForX += GetSingleton(InputManager)->GetMouseDelta().y * CAMERA_ROTATION_SPEED;
				 if (m_cameraRotationForX <= -90)	m_cameraRotationForX = -90.f + 0.0001f;
			else if (m_cameraRotationForX >= 90)	m_cameraRotationForX = 90.f - 0.0001f;
		}
		
		D3DXMATRIX rX, rY;
		D3DXVECTOR3 dir;
		D3DXMatrixRotationX(&rX, D3DXToRadian(m_cameraRotationForX));
		D3DXMatrixRotationY(&rY, D3DXToRadian(m_cameraRotationForY));
		D3DXVec3TransformNormal(&dir, &D3DXVECTOR3(0, 0, 1), &(rX * rY));
		
		GetSingleton(CameraManager)->SetCameraPosition(m_cameraPos);
		GetSingleton(CameraManager)->SetCameraDirection(dir);
	}



	D3DXVECTOR3& objectPos(m_pos);
	D3DXVECTOR2& objectRotation(m_rot);

	CONST FLOAT OBJECT_MOVE_SPEED = 0.1f;
	CONST FLOAT OBJECT_ROTATION_SPEED = 1.2f;
	
	auto ObjectMove = [](D3DXVECTOR3& objectPosition, float rotationDegree, D3DXVECTOR3 dir, float speed)
	{
		D3DXMATRIX r;
		D3DXMatrixRotationY(&r, D3DXToRadian(rotationDegree));
	};
	if (GetSingleton(InputManager)->IsKeyPressed(VK_NUMPAD8))	objectPos.z += OBJECT_MOVE_SPEED;
	if (GetSingleton(InputManager)->IsKeyPressed(VK_NUMPAD5))	objectPos.z -= OBJECT_MOVE_SPEED;
	if (GetSingleton(InputManager)->IsKeyPressed(VK_NUMPAD4))	objectPos.x -= OBJECT_MOVE_SPEED;
	if (GetSingleton(InputManager)->IsKeyPressed(VK_NUMPAD6))	objectPos.x += OBJECT_MOVE_SPEED;

	if (GetSingleton(InputManager)->IsKeyPressed(VK_UP))	objectRotation.x -= OBJECT_ROTATION_SPEED;
	if (GetSingleton(InputManager)->IsKeyPressed(VK_DOWN))	objectRotation.x += OBJECT_ROTATION_SPEED;
	if (GetSingleton(InputManager)->IsKeyPressed(VK_LEFT))	objectRotation.y -= OBJECT_ROTATION_SPEED;
	if (GetSingleton(InputManager)->IsKeyPressed(VK_RIGHT))	objectRotation.y += OBJECT_ROTATION_SPEED;



	if (GetSingleton(InputManager)->IsKeyPressed('R'))
	{
		m_pos = D3DXVECTOR3(0, 0, 0);
		m_rot = D3DXVECTOR2(0, 0);
	}





}

void TestScene::Render()
{
	//Grid
	{
		struct V_PC
		{
			D3DXVECTOR3 p;
			D3DCOLOR c;
			enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };

			V_PC(D3DXVECTOR3 _p, D3DXCOLOR _c) : p(_p), c(_c) {}
		};

		std::vector<V_PC> vC;

		CONST INT SIZE = 10;
		CONST INT n = 5;
		for (INT i = -SIZE; i <= SIZE; i++)
		{
			vC.push_back(V_PC(D3DXVECTOR3(+SIZE, 0, i), i % n ? D3DXCOLOR(0.5f, 0.5f, 0.5f, 1) : D3DXCOLOR(1, 0, 0, 1)));
			vC.push_back(V_PC(D3DXVECTOR3(-SIZE, 0, i), i % n ? D3DXCOLOR(0.5f, 0.5f, 0.5f, 1) : D3DXCOLOR(1, 0, 0, 1)));
			vC.push_back(V_PC(D3DXVECTOR3(i, 0, +SIZE), i % n ? D3DXCOLOR(0.5f, 0.5f, 0.5f, 1) : D3DXCOLOR(0, 0, 1, 1)));
			vC.push_back(V_PC(D3DXVECTOR3(i, 0, -SIZE), i % n ? D3DXCOLOR(0.5f, 0.5f, 0.5f, 1) : D3DXCOLOR(0, 0, 1, 1)));
		}
		vC.push_back(V_PC(D3DXVECTOR3(0, +SIZE, 0), D3DXCOLOR(0, 1, 0, 1)));
		vC.push_back(V_PC(D3DXVECTOR3(0, -SIZE, 0), D3DXCOLOR(0, 1, 0, 1)));

		D3DXMATRIX identity;
		D3DXMatrixIdentity(&identity);
		DEVICE->SetFVF(V_PC::FVF);
		DEVICE->SetTransform(D3DTS_WORLD, &identity);
		DEVICE->SetRenderState(D3DRS_LIGHTING, false);
		DEVICE->DrawPrimitiveUP(D3DPT_LINELIST, vC.size() / 2, &vC[0], sizeof(V_PC));
		DEVICE->SetRenderState(D3DRS_LIGHTING, true);
	}

	D3DXMATRIX rx, ry, tm;
	D3DXMatrixRotationX(&rx, D3DXToRadian(m_rot.x));
	D3DXMatrixRotationY(&ry, D3DXToRadian(m_rot.y));
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);

	D3DXMATRIX m;
	D3DXMatrixIdentity(&m);
	DEVICE->SetTransform(D3DTS_WORLD, &m);
	DEVICE->SetFVF(GetSingleton(Plane)->GetFVF());
	GetSingleton(Plane)->Render(DEVICE);
	//return;



	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	goto a;
	m_model2->SetFVF(DEVICE);
	UINT attributeCount = m_model2->GetAttributeCount();
	for (UINT attribute = 0; attribute < attributeCount; attribute++)
	{
		GeomObj* geomObj = m_model2->GetGeomObj(attribute);
		DEVICE->SetTransform(D3DTS_WORLD, &(geomObj->localMatrix));
		GetSingleton(TempShaderHelper)->SetMaterial("g_material", geomObj->mtrlTex->first);
		GetSingleton(TempShaderHelper)->SetTexture("g_texture", geomObj->mtrlTex->second);

		UINT passCount = GetSingleton(TempShaderHelper)->Begin(DEVICE);
		for (UINT i = 0; i < passCount; i++)
		{
			GetSingleton(TempShaderHelper)->BeginDraw(i);
			m_model2->Render(DEVICE, attribute);
			GetSingleton(TempShaderHelper)->EndDraw();
		}
		GetSingleton(TempShaderHelper)->End();
	}

	m_model1->SetFVF(DEVICE);
	 attributeCount = m_model1->GetAttributeCount();
	for (UINT attribute = 0; attribute < attributeCount; attribute++)
	{
		D3DXMATRIX a;
		D3DXMatrixRotationY(&a, m_rot.y);
		D3DXMATRIX rm;
		{
			D3DXVECTOR3 objDir(0, 0, 1);
			D3DXVec3TransformNormal(&objDir, &objDir, &a);

			
			D3DXVec3Normalize(&objDir, &objDir);
			D3DXVECTOR3 cameraDir(-m_cameraPos);// (GetSingleton(CameraManager)->GetData().direction);

			D3DXMATRIX temp;
			D3DXMatrixLookAtLH(&temp, &D3DXVECTOR3(0, 0, 0), &objDir, &D3DXVECTOR3(0, 1, 0));

			D3DXVECTOR3 coordCameraDir;
			D3DXVec3TransformNormal(&coordCameraDir, &-cameraDir, &temp);
			D3DXVec3Normalize(&coordCameraDir, &D3DXVECTOR3(coordCameraDir.x, coordCameraDir.y, 0));

			float zRotRadian = acos(D3DXVec3Dot(&D3DXVECTOR3(0, 1, 0), &coordCameraDir));
			D3DXVECTOR3 crossResult;
			D3DXVec3Cross(&crossResult, &D3DXVECTOR3(0, 1, 0), &coordCameraDir);
			if (crossResult.z > 0)
				zRotRadian = D3DXToRadian(360) - zRotRadian;

			D3DXMatrixRotationAxis(&rm, &D3DXVECTOR3(0, 0, 1), -zRotRadian);
		}


		GeomObj* geomObj = m_model1->GetGeomObj(attribute);
		DEVICE->SetTransform(D3DTS_WORLD, &(geomObj->localMatrixSR * (rm * a) * geomObj->localMatrixT * tm));
		GetSingleton(TempShaderHelper)->SetMaterial("g_material", geomObj->mtrlTex->first);
		GetSingleton(TempShaderHelper)->SetTexture("g_texture", geomObj->mtrlTex->second);

		UINT passCount = GetSingleton(TempShaderHelper)->Begin(DEVICE);
		for (UINT i = 0; i < passCount; i++)
		{
			GetSingleton(TempShaderHelper)->BeginDraw(i);
			m_model1->Render(DEVICE, attribute);
			GetSingleton(TempShaderHelper)->EndDraw();
		}
		GetSingleton(TempShaderHelper)->End();
	}
	a:


	D3DXMATRIX vm, pm;
	DEVICE->GetTransform(D3DTS_VIEW, &vm);
	DEVICE->GetTransform(D3DTS_PROJECTION, &pm);
	//D3DXMatrixInverse(&invView, 0, &invView);
	//D3DXMatrixInverse(&invProj, 0, &invProj);
	//D3DXMatrixInverse(&invWorld, 0, &invWorld);



	LPD3DXEFFECT sh = nullptr;
	LPD3DXBUFFER buf = nullptr;
	D3DXCreateEffectFromFileW(DEVICE, L"./Resource/d3dxfont.fx", nullptr, nullptr, D3DXSHADER_DEBUG, nullptr, &sh, &buf);
	if (!sh && buf)
		MessageBoxA(g_processManager->GetWndInfo()->hWnd, (LPCSTR)buf->GetBufferPointer(), nullptr, NULL);
	UINT p;


	sh->SetMatrix("g_vM", &vm);
	sh->SetMatrix("g_pM", &pm);

	D3DXMATRIX tsm, trm, ttm;
	D3DXMatrixScaling(&tsm, 0.01, 0.01, 0.01);
	//D3DXMatrixScaling(&tsm, 1, 1, 1);
	D3DXMatrixRotationZ(&trm, D3DXToRadian(180));
	D3DXMatrixTranslation(&ttm, 100, 100, 0);


	sh->SetMatrix("g_wM", &(tsm * trm));

	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	sh->Begin(&p, NULL);
	
	
	LPD3DXSPRITE sp;
	D3DXCreateSprite(DEVICE, &sp);
	sp->Begin(D3DXSPRITE_OBJECTSPACE | D3DXSPRITE_DONOTMODIFY_RENDERSTATE | D3DXSPRITE_DO_NOT_ADDREF_TEXTURE);
	auto DrawFont = [&sp, &sh](LPD3DXFONT font, UINT x, UINT y, std::wstring& text, const D3DXCOLOR& color = D3DXCOLOR(0, 0, 0, 1))
	{
		sh->BeginPass(0);
		RECT rc;
		SetRect(&rc, x, y, 0, 0);
		font->DrawTextW(sp, text.data(), -1, &rc, DT_NOCLIP | DT_LEFT | DT_BOTTOM, D3DXCOLOR(0, 0, 0, 1));
		sh->EndPass();
		
	};
	DrawFont(m_font, 0, 0, std::wstring(L"¾È³ç1234¡Ä./"));
	sp->End();
	sp->Release();

	sh->End();
	sh->Release();
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
}

void TestScene::Release()
{
	SAFE_DELETE(m_model1);
	SAFE_DELETE(m_model2);
	SAFE_RELEASE(m_font);
}
