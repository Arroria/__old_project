#include "stdafx.h"
#include "PlaneRenderer.h"


PlaneRenderer::PlaneRenderer()
{
	std::array<std::pair<D3DXVECTOR3, D3DXVECTOR2>, 4> planeVertex;
	planeVertex[0] = std::make_pair(D3DXVECTOR3(-0.5f, 0.5f, 0), D3DXVECTOR2(0, 0));
	planeVertex[1] = std::make_pair(D3DXVECTOR3(0.5f, 0.5f, 0), D3DXVECTOR2(1, 0));
	planeVertex[2] = std::make_pair(D3DXVECTOR3(-0.5f, -0.5f, 0), D3DXVECTOR2(0, 1));
	planeVertex[3] = std::make_pair(D3DXVECTOR3(0.5f, -0.5f, 0), D3DXVECTOR2(1, 1));

	m_texPlane[0] = Vertex_PT(planeVertex[0].first, planeVertex[0].second);
	m_texPlane[1] = Vertex_PT(planeVertex[1].first, planeVertex[1].second);
	m_texPlane[2] = Vertex_PT(planeVertex[3].first, planeVertex[3].second);
	m_texPlane[3] = Vertex_PT(planeVertex[0].first, planeVertex[0].second);
	m_texPlane[4] = Vertex_PT(planeVertex[3].first, planeVertex[3].second);
	m_texPlane[5] = Vertex_PT(planeVertex[2].first, planeVertex[2].second);

	ZeroMemory(&m_whiteMtrl, sizeof(D3DMATERIAL9));
	m_whiteMtrl.Diffuse = m_whiteMtrl.Ambient = m_whiteMtrl.Specular = D3DXCOLOR(1, 1, 1, 1);


	g_processManager->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_processManager->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_processManager->GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	D3DXCreateSprite(DEVICE, &m_sprite);
}
PlaneRenderer::~PlaneRenderer()
{
	SAFE_RELEASE(m_sprite);
}


void PlaneRenderer::Render(LPDIRECT3DDEVICE9 device, Texture * texture, const D3DXVECTOR3 & position, const bool& drawReverse, const D3DXQUATERNION& rotation, const D3DXVECTOR2 & scale) const
{
	D3DXMATRIX s, r, t;
	D3DXMatrixScaling(&s, scale.x * texture->GetImageInfo().Width * (drawReverse ? -1 : 1), scale.y * texture->GetImageInfo().Height, 1);
	D3DXMatrixRotationQuaternion(&r, &rotation);
	D3DXMatrixTranslation(&t, position.x, position.y, position.z);
	device->SetTransform(D3DTS_WORLD, &(s * r * t));
	device->SetTexture(0, texture->GetTexture());
	device->SetMaterial(&m_whiteMtrl);
	device->SetFVF(Vertex_PT::FVF);
	device->SetRenderState(D3DRS_LIGHTING, false);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_ZWRITEENABLE, false);

	device->SetRenderState(D3DRS_CULLMODE, drawReverse ? D3DCULL_CW : D3DCULL_CCW);

	device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_texPlane.size() / 3, &m_texPlane[0], sizeof(Vertex_PT));
	device->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void PlaneRenderer::SpriteRender(Texture * texture, const D3DXVECTOR3 & position, const D3DXQUATERNION & rotation, const D3DXVECTOR2 & scale, const float& wwwwwww) const
{
	D3DXMATRIX s, r, t;
	D3DXMatrixScaling(&s, scale.x, scale.y, 1);
	D3DXMatrixRotationQuaternion(&r, &rotation);
	D3DXMatrixTranslation(&t, position.x, position.y, position.z);

	m_sprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_sprite->SetTransform(&(s * r * t));
	//if (wwwwwww == 1)
	//	m_sprite->Draw(texture->GetTexture(), nullptr, &D3DXVECTOR3(texture->GetImageInfo().Width * 0.5f, texture->GetImageInfo().Height * 0.5f, 0), nullptr, D3DXCOLOR(1, 1, 1, 1));
	//else
	{
		RECT temp;
		SetRect(&temp, 0, 0, texture->GetImageInfo().Width * wwwwwww, texture->GetImageInfo().Height);
		m_sprite->Draw(texture->GetTexture(), &temp, &D3DXVECTOR3(texture->GetImageInfo().Width * 0.5f, texture->GetImageInfo().Height * 0.5f, 0), nullptr, D3DXCOLOR(1, 1, 1, 1));
	}
	m_sprite->End();
}
