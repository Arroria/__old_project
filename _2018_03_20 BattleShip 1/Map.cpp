#include "DXUT.h"
#include "Map.h"


Map::Map(const D3DXVECTOR3& pos)
	: m_pos(pos)
{
	m_map.fill(std::make_pair(nullptr, false));

	LoadTex(L"./Resource/Tile_0.png", L"Tile_Void");
	LoadTex(L"./Resource/Tile_1.png", L"Tile_Red");
	LoadTex(L"./Resource/Tile_3.png", L"Tile_Green");
	LoadTex(L"./Resource/Tile_Selected.png", L"Tile_Selete");
}
Map::~Map()
{
}



void Map::Render(const bool& viewCursor)
{
	Texture* texVoid = GetTex(L"Tile_Void");
	Texture* texRed = GetTex(L"Tile_Red");
	Texture* texGreen = GetTex(L"Tile_Green");
	Texture* texSelete = GetTex(L"Tile_Selete");

	for (UINT x = 0; x < 10; x++)
	{
		for (UINT y = 0; y < 10; y++)
		{
			auto& tile = GetTile(x, y);

			Texture* tex = tile.second ?
				(tile.first ? texGreen : texRed) :
				texVoid;

			DEVICE->SetMaterial(&WHITEMTRL);
			DrawTile(x, y, D3DXVECTOR3(0, 0, 0), tex);
		}
	}

	if (viewCursor)
	{
		POINT tilePos;
		if (GetCursorTile(tilePos))
		{
			DEVICE->SetMaterial(&WHITEMTRL);
			DrawTile(tilePos.x, tilePos.y, D3DXVECTOR3(0, 0.1, 0), texSelete);
		}
	}
}



void Map::DrawTile(const UINT& x, const UINT& y, const D3DXVECTOR3& offset, Texture * tex)
{
	D3DXMATRIX rm, tm;
	D3DXMatrixRotationX(&rm, D3DXToRadian(90));
	D3DXMatrixTranslation(&tm, x - 4.5 + m_pos.x + offset.x, 0 + m_pos.y + offset.y, 4.5 - y + m_pos.z + offset.z);

	DEVICE->SetTransform(D3DTS_WORLD, &(rm * tm));
	DEVICE->SetTexture(0, tex->tex);
	DEVICE->SetFVF(Plane::GetFVF());
	PlaneRender();
}



void Map::RegistShip(const UINT & x, const UINT & y, Ship * shipPtr)
{
	GetTile(x, y).first = shipPtr;
}

bool Map::GetCursorTile(POINT& pos)
{
	const POINT& mousePos = SingletonInstance(InputManager)->GetMousePos();
	D3DXMATRIX viewInvM; {
		D3DXMATRIX viewM;
		DEVICE->GetTransform(D3DTS_VIEW, &viewM);
		D3DXMatrixInverse(&viewInvM, 0, &viewM);
	}
	D3DXVECTOR3 rayPos, rayDir; {
		D3DVIEWPORT9 viewPort;
		DEVICE->GetViewport(&viewPort);
		D3DXMATRIX projM;
		DEVICE->GetTransform(D3DTS_PROJECTION, &projM);
		
		rayDir.x = (mousePos.x * 2.f / viewPort.Width - 1) / projM._11;
		rayDir.y = (mousePos.y * -2.f / viewPort.Height + 1) / projM._22;
		rayDir.z = 1;

		rayPos.x = viewInvM._41;
		rayPos.y = viewInvM._42;
		rayPos.z = viewInvM._43;
		D3DXVec3TransformNormal(&rayDir, &rayDir, &viewInvM);
		D3DXVec3Normalize(&rayDir, &rayDir);
	}
	D3DXVECTOR3 plane[4]; {
		plane[0] = D3DXVECTOR3(-0.5f, 0, +0.5f) * 10 + m_pos;
		plane[1] = D3DXVECTOR3(+0.5f, 0, +0.5f) * 10 + m_pos;
		plane[2] = D3DXVECTOR3(-0.5f, 0, -0.5f) * 10 + m_pos;
		plane[3] = D3DXVECTOR3(+0.5f, 0, -0.5f) * 10 + m_pos;
	}
	float u, v, distance;
	if (D3DXIntersectTri(&plane[0], &plane[1], &plane[3], &rayPos, &rayDir, &u, &v, &distance) ||
		D3DXIntersectTri(&plane[0], &plane[3], &plane[2], &rayPos, &rayDir, &u, &v, &distance))
	{
		D3DXVECTOR3 result = rayPos + rayDir * distance;
		result -= m_pos;

		pos.x = result.x + 5;
		pos.y = 5 - result.z;
			 if (pos.x < 0)		pos.x = 0;
		else if (pos.x >= 10)	pos.x = 9;
			 if (pos.y < 0)		pos.y = 0;
		else if (pos.y >= 10)	pos.y = 9;

		return true;
	}
	return false;
}
