#pragma once
#include "Singleton.h"
class PlaneRenderer :
	public Singleton<PlaneRenderer>
{
private:
	struct Vertex_PT
	{
		D3DXVECTOR3 p;
		D3DXVECTOR2 uv;

		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_TEX1
		};
		Vertex_PT() {}
		Vertex_PT(const D3DXVECTOR3& _p, const D3DXVECTOR2& _uv) : p(_p), uv(_uv) {}
	};
	
private:
	std::array<Vertex_PT, 6> m_texPlane;
	D3DMATERIAL9 m_whiteMtrl;

	LPD3DXSPRITE m_sprite;

public:
	void Render(LPDIRECT3DDEVICE9 device, Texture* texture, const D3DXVECTOR3& position, const bool& drawReverse = false, const D3DXQUATERNION& rotation = D3DXQUATERNION(0, 0, 0, 1), const D3DXVECTOR2& scale = D3DXVECTOR2(1, 1)) const;
	void SpriteRender(Texture* texture, const D3DXVECTOR3& position, const D3DXQUATERNION& rotation = D3DXQUATERNION(0, 0, 0, 1), const D3DXVECTOR2& scale = D3DXVECTOR2(1, 1), const float& wwwwwww = 1) const;

public:
	PlaneRenderer();
	~PlaneRenderer();
};

