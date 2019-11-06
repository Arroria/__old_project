#include "DXUT.h"
#include "Map.h"

#include "Ship.h"


Map::Map()
	: m_pos(0, 0, 0)
{
}
Map::~Map()
{
}



void Map::RenderMap()
{
	for (UINT tileIndex = 0; tileIndex < 100; tileIndex++)
	{
		Tile& tile = GetTile(tileIndex);
		
		Texture* tex = TextureGet(
			tile.second ?
			(tile.first ? L"TileT" : L"TileF") :
			L"Tile"
		);
		DEVICE->SetTexture(0, tex->tex);

		D3DXVECTOR3 tilePos = LineToVec(tileIndex);
		D3DXMATRIX rm, tm;
		D3DXMatrixRotationX(&rm, RADIAN(90));
		D3DXMatrixTranslation(&tm, tilePos.x, tilePos.y, tilePos.z);
		DEVICE->SetTransform(D3DTS_WORLD, &(rm * tm));
		
		SetWhiteMaterial();
		SingletonInstance(Plane)->Render();
	}
}

void Map::RenderTile(const UINT & pos, Texture * tex)
{
	DEVICE->SetTexture(0, tex->tex);

	D3DXVECTOR3 tilePos = LineToVec(pos);
	D3DXMATRIX rm, tm;
	D3DXMatrixRotationX(&rm, RADIAN(90));
	D3DXMatrixTranslation(&tm, tilePos.x, tilePos.y, tilePos.z);
	DEVICE->SetTransform(D3DTS_WORLD, &(rm * tm));

	SetWhiteMaterial();
	SingletonInstance(Plane)->Render();
}



bool Map::GetMousePos(POINT & out)
{
	D3DXVECTOR3 rayPos, rayDir; {
		D3DXMATRIX viewM, viewInvM, projM;
		DEVICE->GetTransform(D3DTS_VIEW, &viewM);
		DEVICE->GetTransform(D3DTS_PROJECTION, &projM);
		D3DXMatrixInverse(&viewInvM, 0, &viewM);

		D3DVIEWPORT9 viewPort;
		DEVICE->GetViewport(&viewPort);

		POINT mousePos = SingletonInstance(InputManager)->GeMousePos();

		rayDir.x = ((mousePos.x * 2.f) / viewPort.Width - 1) / projM._11;
		rayDir.y = ((mousePos.y * -2.f) / viewPort.Height + 1) / projM._22;
		rayDir.z = 1;
		D3DXVec3TransformNormal(&rayDir, &rayDir, &viewInvM);
		D3DXVec3Normalize(&rayDir, &rayDir);

		rayPos.x = viewInvM._41;
		rayPos.y = viewInvM._42;
		rayPos.z = viewInvM._43;
	}

	constexpr FLOAT TILESIZE = 10;
	D3DXVECTOR3 plane[4];
	plane[0] = D3DXVECTOR3(-TILESIZE / 2, 0, +TILESIZE / 2) + m_pos;
	plane[1] = D3DXVECTOR3(+TILESIZE / 2, 0, +TILESIZE / 2) + m_pos;
	plane[2] = D3DXVECTOR3(-TILESIZE / 2, 0, -TILESIZE / 2) + m_pos;
	plane[3] = D3DXVECTOR3(+TILESIZE / 2, 0, -TILESIZE / 2) + m_pos;

	float u, v, dist;
	if (D3DXIntersectTri(&plane[0], &plane[1], &plane[2], &rayPos, &rayDir, &u, &v, &dist) ||
		D3DXIntersectTri(&plane[1], &plane[3], &plane[2], &rayPos, &rayDir, &u, &v, &dist))
	{
		D3DXVECTOR3 terminal = rayPos + rayDir * dist;
		terminal -= m_pos - D3DXVECTOR3(TILESIZE / 2, 0, TILESIZE / 2);

		out.x = terminal.x;
		out.y = terminal.z;

		if		(out.x < 0)	out.x = 0;
		else if (out.x > 9) out.x = 9;
		if		(out.y < 0)	out.y = 0;
		else if (out.y > 9) out.y = 9;
		return true;
	}
	else
	{
		out.x = -1;
		out.y = -1;
		return false;
	}
}



void Map::ClearMap()
{
	m_tile.fill(std::make_pair( nullptr, false ));
}

bool Map::RegistShip(Ship * ship, const bool& lookAtY, const POINT & pos, const bool& onlyCheck)
{
	UINT size = ship->GetSize();

	/* Check */ {
		POINT checkPos = pos;
		auto& upper = (lookAtY ? checkPos.y : checkPos.x);
		for (UINT i = 0; i < size; i++)
		{
			if (checkPos.x < 0 ||
				checkPos.y < 0 ||
				checkPos.x > 9 ||
				checkPos.y > 9 ||
				GetTile(checkPos).first)
				return false;
			upper++;
		}
	}

	if (onlyCheck)
		return true;

	/* Check */ {
		POINT insertPos = pos;
		auto& upper = (lookAtY ? insertPos.y : insertPos.x);
		for (UINT i = 0; i < size; i++)
		{
			GetTile(insertPos).first = ship;
			upper++;
		}
	}
}
