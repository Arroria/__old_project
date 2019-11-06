#include "DXUT.h"
#include "Map.h"

#include "Ship.h"

Map::Map()
	: m_pos(0, 0, 0)
{
	ZeroMap();

	TextureLoad(L"./Resource/map/Tile_0.png", L"Tile_U");
	TextureLoad(L"./Resource/map/Tile_1.png", L"Tile_T");
	TextureLoad(L"./Resource/map/Tile_2.png", L"Tile_F");
	TextureLoad(L"./Resource/map/Tile_s.png", L"Tile_S");
}
Map::~Map()
{
}



void Map::Update()
{
}

void Map::RenderMap()
{
	DEVICE->SetRenderState(D3DRS_LIGHTING, false);

	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	D3DXMATRIX planeM; {
		D3DXMatrixRotationX(&planeM, D3DXToRadian(90));
	}
	for (UINT y = 0; y < 10; y++)
	{
		for (UINT x = 0; x < 10; x++)
		{
			Tile& tile = GetTile(x, y);
			Texture* tex = nullptr; {
				if (tile.second || SingletonInstance(InputManager)->IsKeyPressed(VK_NUMPAD1))
				{
					tex = tile.first ?
						TextureGet(L"Tile_T") :
						TextureGet(L"Tile_F");
				}
				else
					tex = TextureGet(L"Tile_U");
			}

			D3DXMATRIX tm; {
				D3DXVECTOR3 tilePos = PointToVector(x, y);
				D3DXMatrixTranslation(&tm, tilePos.x, tilePos.y, tilePos.z);
			}

			DEVICE->SetTexture(0, tex->tex);
			DEVICE->SetFVF(Plane::GetFVF());
			DEVICE->SetTransform(D3DTS_WORLD, &(planeM * tm));
			SingletonInstance(Plane)->Render(DEVICE);
		}
	}
}

void Map::RenderTile(const POINT& pos, Texture* texture)
{
	Texture* tex = texture ?
		texture :
		TextureGet(L"Tile_S");

	D3DXMATRIX planeM; {
		D3DXMatrixRotationX(&planeM, D3DXToRadian(90));
	}
	D3DXMATRIX tm; {
		D3DXVECTOR3 tilePos = PointToVector(pos);
		D3DXMatrixTranslation(&tm, tilePos.x, tilePos.y, tilePos.z);
	}
	tm._42 += 0.05f;

	DEVICE->SetTexture(0, tex->tex);
	DEVICE->SetFVF(Plane::GetFVF());
	DEVICE->SetTransform(D3DTS_WORLD, &(planeM * tm));
	SingletonInstance(Plane)->Render(DEVICE);
}



bool Map::RayCastInTile(POINT& pos)
{
	D3DXVECTOR3 rayPos;
	D3DXVECTOR3 rayDir; {
		D3DXMATRIX viewInvM;
		DEVICE->GetTransform(D3DTS_VIEW, &viewInvM);
		D3DXMatrixInverse(&viewInvM, 0, &viewInvM);

		D3DXMATRIX projM;		DEVICE->GetTransform(D3DTS_PROJECTION, &projM);
		D3DVIEWPORT9 viewPort;	DEVICE->GetViewport(&viewPort);

		POINT mousePos = SingletonInstance(InputManager)->GetMousePos();
		rayDir.x = ((mousePos.x * 2.f) / viewPort.Width - 1) / projM._11;
		rayDir.y = ((mousePos.y * -2.f) / viewPort.Height + 1) / projM._22;
		rayDir.z = 1; 
		
		D3DXVec3TransformNormal(&rayDir, &rayDir, &viewInvM);
		D3DXVec3Normalize(&rayDir, &rayDir);


		rayPos.x = viewInvM._41;
		rayPos.y = viewInvM._42;
		rayPos.z = viewInvM._43;
	}

	D3DXVECTOR3 plane[4];
	plane[0] = D3DXVECTOR3(-0.5f * 10 + m_pos.x, 0 + m_pos.y, m_pos.z +  +0.5f * 10);
	plane[1] = D3DXVECTOR3(+0.5f * 10 + m_pos.x, 0 + m_pos.y, m_pos.z +  +0.5f * 10);
	plane[2] = D3DXVECTOR3(-0.5f * 10 + m_pos.x, 0 + m_pos.y, m_pos.z +  -0.5f * 10);
	plane[3] = D3DXVECTOR3(+0.5f * 10 + m_pos.x, 0 + m_pos.y, m_pos.z +  -0.5f * 10);

	float u, v, distance;
	if (D3DXIntersectTri(&plane[0], &plane[1], &plane[2], &rayPos, &rayDir, &u, &v, &distance) ||
		D3DXIntersectTri(&plane[1], &plane[3], &plane[2], &rayPos, &rayDir, &u, &v, &distance))
	{
		D3DXVECTOR3	result = rayPos + rayDir * distance;
		result -= m_pos;

		pos.x = result.x + 5;
		pos.y = result.z + 5;
			 if (pos.x < 0) pos.x = 0;
		else if (pos.x > 9) pos.x = 9;
			 if (pos.y < 0) pos.y = 0;
		else if (pos.y > 9) pos.y = 9;

		return true;
	}
	else
	{
		pos.x = -1;
		pos.y = -1;
		return false;
	}
}



Map::Tile& Map::GetTile(const UINT & x, const UINT & y)	{ return m_tile[PointToLine(x, y)]; }
Map::Tile& Map::GetTile(const POINT & pos)				{ return m_tile[PointToLine(pos)]; }
Map::Tile& Map::GetTile(const UINT & pos)				{ return m_tile[pos]; }
UINT Map::PointToLine(const UINT & x, const UINT & y)	{ return x + y * 10; }
UINT Map::PointToLine(const POINT & pos)				{ return pos.x + pos.y * 10; }
POINT Map::LineToPoint(const UINT & pos)				{ POINT result; result.x = pos % 10; result.y = pos / 10; return result; }
D3DXVECTOR3 Map::PointToVector(const UINT & x, const UINT & y)	{ return D3DXVECTOR3(m_pos.x + x - 4.5f			, m_pos.y , m_pos.z + y - 4.5f); }
D3DXVECTOR3 Map::PointToVector(const POINT & pos)				{ return D3DXVECTOR3(m_pos.x + pos.x - 4.5f		, m_pos.y , m_pos.z + pos.y - 4.5f); }
D3DXVECTOR3 Map::LineToVector(const UINT & pos)					{ return D3DXVECTOR3(m_pos.x + (pos % 10) - 4.5f, m_pos.y , m_pos.z + (pos / 10) - 4.5f); }

bool Map::Hit(const POINT & pos)
{
	GetTile(pos).second = true;

	if (GetTile(pos).first)
	{
		GetTile(pos).first->Hit();
		return true;
	}
	return false;
}



void Map::ZeroMap()	{ m_tile.fill(std::make_pair( nullptr, false )); }

bool Map::RegistShip(Ship* ship, const POINT& pos, const bool& lookAtY, const bool& justCheck)
{
	UINT size = ship->GetSize();

	if (pos.x < 0 &&
		pos.x > 10 - (lookAtY ? 1 : size) &&
		pos.y < 0 &&
		pos.y > 10 - (lookAtY ? size : 1))
		return false;

	/* Check */ {
		POINT _pos = pos;
		auto& xOrY = lookAtY ? _pos.y : _pos.x;
		for (UINT i = 0; i < size; i++)
		{
			if (GetTile(_pos).first)
				return false;
			xOrY++;
		}
	}

	/* Insert */
	if (!justCheck)
	{
		POINT _pos = pos;
		auto& xOrY = lookAtY ? _pos.y : _pos.x;
		for (UINT i = 0; i < size; i++)
		{
			GetTile(_pos).first = ship;
			xOrY++;
		}
	}
	return true;
}
