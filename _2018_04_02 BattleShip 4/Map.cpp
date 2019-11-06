#include "DXUT.h"
#include "Map.h"


Map::Map()
	: m_pos(0, 0, 0)
{
	MapClear();
}
Map::~Map()
{
}



void Map::RenderMap(const bool & myTurn, Texture* tex)
{
	for (UINT i = 0; i < 100; i++)
	{
		Texture* tex =
			m_tile[i].second ?
			(m_tile[i].first ? TextureGet(L"TileT") : TextureGet(L"TileF")) :
			TextureGet(L"Tile");
		RenderTile(LineToPoint(i), tex);
	}

	if (!myTurn)
		return;

	D3DXMATRIX sm, rm, tm;
	D3DXMatrixScaling(&sm, 13, 13, 13);
	D3DXMatrixRotationX(&rm, D3DXToRadian(90));
	D3DXMatrixTranslation(&tm, m_pos.x, m_pos.y, m_pos.z);

	DEVICE->SetMaterial(&WHITEMTRL);
	DEVICE->SetTexture(0, tex->tex);
	DEVICE->SetTransform(D3DTS_WORLD, &(sm * rm * tm));
	SingletonInstance(Plane)->Render();
}



void Map::RenderTile(const POINT & tilePos, Texture * tex)
{
	D3DXVECTOR3 tileWorldPos = PointToVec(tilePos);
	
	D3DXMATRIX rm, tm;
	D3DXMatrixRotationX(&rm, D3DXToRadian(90));
	D3DXMatrixTranslation(&tm, tileWorldPos.x, tileWorldPos.y, tileWorldPos.z);

	DEVICE->SetMaterial(&WHITEMTRL);
	DEVICE->SetTexture(0, tex->tex);
	DEVICE->SetTransform(D3DTS_WORLD, &(rm * tm));
	SingletonInstance(Plane)->Render();
}



bool Map::GetCursor(POINT & out)
{
	D3DXVECTOR3 rayDir, rayPos; {
		D3DVIEWPORT9 viewPort;
		D3DXMATRIX viewInvM, projM;
		DEVICE->GetViewport(&viewPort);
		DEVICE->GetTransform(D3DTS_PROJECTION, &projM);
		DEVICE->GetTransform(D3DTS_VIEW, &viewInvM);
		D3DXMatrixInverse(&viewInvM, 0, &viewInvM);

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

	constexpr FLOAT MAPSIZE = 10;
	D3DXVECTOR3 plane[4];
	plane[0] = D3DXVECTOR3(-0.5f, 0, +0.5f) * MAPSIZE + m_pos;
	plane[1] = D3DXVECTOR3(+0.5f, 0, +0.5f) * MAPSIZE + m_pos;
	plane[2] = D3DXVECTOR3(-0.5f, 0, -0.5f) * MAPSIZE + m_pos;
	plane[3] = D3DXVECTOR3(+0.5f, 0, -0.5f) * MAPSIZE + m_pos;

	float u, v, dist;
	if (D3DXIntersectTri(&plane[0], &plane[1], &plane[2], &rayPos, &rayDir, &u, &v, &dist) ||
		D3DXIntersectTri(&plane[1], &plane[3], &plane[2], &rayPos, &rayDir, &u, &v, &dist))
	{
		D3DXVECTOR3 hitPos = rayPos + rayDir * dist;
		hitPos -= m_pos;

		out.x = (hitPos.x + (MAPSIZE / 2));
		out.y = (hitPos.z + (MAPSIZE / 2));

			if (out.x < 0)	out.x = 0;
		else if (out.x > 9)	out.x = 9;
			if (out.x < 0)	out.y = 0;
		else if (out.x > 9)	out.y = 9;

		return true;
	}
	out.x = out.y = -1;
	return false;
}



void Map::MapClear()
{
	m_tile.fill(std::make_pair(nullptr, false));
}
