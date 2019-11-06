#include "stdafx.h"
#include "DefaultBlock.h"


DefaultBlock::DefaultBlock(const int & blockId, const D3DXVECTOR3 & position, const D3DXVECTOR3& scale, const D3DXVECTOR2& collisionSize, const float & durability, Texture * texture, const bool& breakable, const bool& ObjectMovable)
	: BaseBlock(blockId, position, scale, collisionSize, durability, 0)
	, m_txBlock(texture)
	, m_aseBlock(nullptr)
{
	m_canPenetrateForMove = ObjectMovable;
	m_canPenetrateForAttack = ObjectMovable;
	m_unbreakable = !breakable;
	switch (blockId)
	{
	case 0:
		m_aseBlock = g_ase->GetAse(L"Block_Grass");
		break;
	case 1:
		m_aseBlock = g_ase->GetAse(L"Block_Stone");
		break;
	case 2:
		m_aseBlock = g_ase->GetAse(L"Block_Chest");
		break;
	}
}
DefaultBlock::~DefaultBlock()
{
}


bool DefaultBlock::Update()
{
	if (m_durability <= 0)
	{
		DestroyThis();
		return false;
	}

	return false;
}

bool DefaultBlock::Render()
{
	//D3DXVECTOR3 scale(D3DXVECTOR3(1, 1, 1) * 50);
	//D3DXMATRIX mat1, mat2;
	//D3DXMatrixTranslation(&mat2, m_position.x, m_position.z, m_position.y);
	//D3DXMatrixScaling(&mat1, scale.x, scale.z, scale.y);
	//g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(mat1 * mat2));
	//g_processManager->GetDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	//g_processManager->GetDevice()->SetTexture(0, m_txBlock->d3dxTexture);
	//g_processManager->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	//struct temp
	//{
	//	D3DXVECTOR3 p;
	//	D3DXVECTOR2 uv;
	//	temp(D3DXVECTOR3 _p, D3DXVECTOR2 _uv) : p(_p), uv(_uv) {}
	//};
	//std::vector<temp> tmp;
	//
	//tmp.push_back(temp(D3DXVECTOR3(-1, 1, 1), D3DXVECTOR2(0, 0)));
	//tmp.push_back(temp(D3DXVECTOR3(1, 1, 1), D3DXVECTOR2(1, 0)));
	//tmp.push_back(temp(D3DXVECTOR3(1, 1, -1), D3DXVECTOR2(1, 1)));
	//tmp.push_back(temp(D3DXVECTOR3(-1, 1, 1), D3DXVECTOR2(0, 0)));
	//tmp.push_back(temp(D3DXVECTOR3(1, 1, -1), D3DXVECTOR2(1, 1)));
	//tmp.push_back(temp(D3DXVECTOR3(-1, 1, -1), D3DXVECTOR2(0, 1)));
	//
	//g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, tmp.size() / 3, &tmp[0], sizeof(temp));

	D3DXMATRIX t, s;
	D3DXMatrixTranslation(&t, m_position.x, m_position.z, m_position.y);
	D3DXMatrixScaling(&s, m_scale.x, m_scale.z, m_scale.y);
	m_aseBlock->Render(s * t);
	if (!m_canPenetrateForMove)
	{
		D3DXMatrixTranslation(&t, m_position.x, m_position.z + 10, m_position.y);
		m_aseBlock->Render(s * t);
	}

	///GetSingleton(TextureRenderer)->SetTransform(m_position, m_scale);
	///GetSingleton(TextureRenderer)->DrawTexture(m_txBlock);
	return false;
}
