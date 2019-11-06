#include "stdafx.h"
#include "Yamato.h"

#include "YamatoTurret.h"

Yamato::Yamato()
	: m_model		(GetSingleton(ASELoader)->LoadASE(L"./Resource/enemy", L"Yamato.ASE"))
	, m_turretModel	(GetSingleton(ASELoader)->LoadASE(L"./Resource/enemy", L"Yamato_1.ASE"))

	, m_isDead(false)
{
}
Yamato::~Yamato()
{
	SAFE_DELETE(m_model);
}



void Yamato::Initialize()
{
	D3DXQUATERNION L, R(0, 0, 0, 1);
	D3DXQuaternionRotationAxis(&L, &D3DXVECTOR3(0, 1, 0), D3DXToRadian(180));

	m_weakness.push_back(new YamatoTurret(D3DXVECTOR3(-3.629, 1.715, 15.588)	, L));
	m_weakness.push_back(new YamatoTurret(D3DXVECTOR3(-4.041, 1.714, 7.234)		, L));
	m_weakness.push_back(new YamatoTurret(D3DXVECTOR3(-4.045, 1.716, -2.422)	, L));
	m_weakness.push_back(new YamatoTurret(D3DXVECTOR3(-3.173, 1.716, -13.637)	, L));
	m_weakness.push_back(new YamatoTurret(D3DXVECTOR3(3.629, 1.715, 15.588)		, R));
	m_weakness.push_back(new YamatoTurret(D3DXVECTOR3(4.041, 1.714, 7.234)		, R));
	m_weakness.push_back(new YamatoTurret(D3DXVECTOR3(4.045, 1.716, -2.422)		, R));
	m_weakness.push_back(new YamatoTurret(D3DXVECTOR3(3.173, 1.716, -13.637)	, R));
}

void Yamato::Update(std::vector<EnemyAttack*>* attackContainer, const D3DXVECTOR3& playerPos)
{
	UINT life = 0;
	for (auto& iter : m_weakness)
	{
		iter->Update(m_pos, m_rot, attackContainer, playerPos);
		if (iter->GetHealth())
			life++;
	}
	if (!life)
		m_isDead = true;
}

void Yamato::Render()
{
	D3DXMATRIX rm, tm;
	D3DXMatrixRotationQuaternion(&rm, &m_rot);
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);

	GetSingleton(TempShaderHelper)->SeleteShader(L"Light");
	m_model->SetFVF(DEVICE);
	UINT attributeCount = m_model->GetAttributeCount();
	for (UINT attribute = 0; attribute < attributeCount; attribute++)
	{
		GeomObj* geomObj = m_model->GetGeomObj(attribute);
		GetSingleton(TempShaderHelper)->SetMaterial("g_material", geomObj->mtrlTex->first);
		GetSingleton(TempShaderHelper)->SetTexture("g_texture", geomObj->mtrlTex->second);
		DEVICE->SetTransform(D3DTS_WORLD, &(geomObj->localMatrixSR * rm * geomObj->localMatrixT * tm));

		UINT passCount = GetSingleton(TempShaderHelper)->Begin(DEVICE);
		for (UINT i = 0; i < passCount; i++)
		{
			GetSingleton(TempShaderHelper)->BeginDraw(i);
			m_model->Render(DEVICE, attribute);
			GetSingleton(TempShaderHelper)->EndDraw();
		}
		GetSingleton(TempShaderHelper)->End();
	}

	for (auto& iter : m_weakness)
		iter->Render(m_model->GetGeomObj(0)->localMatrix, rm, tm, m_turretModel);
}

void Yamato::Release()
{
	for (auto& iter : m_weakness)
		SAFE_RELEASE_DELETE(iter);
	m_weakness.clear();
}
