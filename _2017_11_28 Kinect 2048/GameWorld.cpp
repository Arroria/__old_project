#include "stdafx.h"
#include "GameWorld.h"

#include "SettingUI.h"
#define ArrivalTime SettingUI::GetBoxMovementSpeed()

#define BackgroundOutlineRatio 0.02f
#define BackgroundColor D3DXCOLOR(1, 1, 1, 1)
#define BlockOutlineRatio 0.05f

GameWorld::GameWorld(const POINT_ex& worldGridSize)
	: m_worldGridSize(worldGridSize)
	
	, m_timeAfterDeparture(0)
	, m_worldUpdated(false)

	, m_font(nullptr)

	, m_score(0)
	, m_scoreTempStack(0)
{
	for (int y = 0; y < worldGridSize.y; y++)
	{
		for (int x = 0; x < worldGridSize.y; x++)
		{
			m_gameWorld[y][x].first = m_nextWorld[y][x].first = 0;
			m_nextWorld[y][x].second = false;
		}
	}
	CreateBlock();

	D3DXCreateFontW(g_processManager->GetDevice(), -25, 0, FW_DONTCARE, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"MUSECA", &m_font);
}
GameWorld::~GameWorld()
{
}


void GameWorld::Update()
{
	if (m_timeAfterDeparture)
	{
		if ((m_timeAfterDeparture -= g_processManager->GetDeltaTime()) < 0)
		{
			m_timeAfterDeparture = 0;
			m_worldUpdated = true;
		}
	}

	if (m_worldUpdated)
	{
		m_worldUpdated = false;
		m_score += m_scoreTempStack;
		m_scoreTempStack = 0;

		for (int y = 0; y < m_worldGridSize.y; y++)
		{
			for (int x = 0; x < m_worldGridSize.x; x++)
			{
				m_gameWorld[y][x].first = m_nextWorld[y][x].first;
				m_nextWorld[y][x].first = 0;
				m_gameWorld[y][x].second = POINT_ex(x, y);
				m_nextWorld[y][x].second = false;
			}
		}
		CreateBlock();
	}
}

void GameWorld::Render(const D3DXVECTOR2 & drawCenter, const D3DXVECTOR2 & worldSize)
{
	//CreatePlan
	struct TempVertex
	{
		D3DXVECTOR3 pos;
		D3DCOLOR color;
		TempVertex(D3DXVECTOR3 pos, D3DCOLOR color) : pos(pos), color(color) {}
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
	};
	std::vector<TempVertex> temp;
	{
		temp.push_back(TempVertex(D3DXVECTOR3(-1, +1, 0), D3DXCOLOR(1, 1, 1, 1)));
		temp.push_back(TempVertex(D3DXVECTOR3(+1, -1, 0), D3DXCOLOR(1, 1, 1, 1)));
		temp.push_back(TempVertex(D3DXVECTOR3(+1, +1, 0), D3DXCOLOR(1, 1, 1, 1)));
		temp.push_back(TempVertex(D3DXVECTOR3(-1, +1, 0), D3DXCOLOR(1, 1, 1, 1)));
		temp.push_back(TempVertex(D3DXVECTOR3(-1, -1, 0), D3DXCOLOR(1, 1, 1, 1)));
		temp.push_back(TempVertex(D3DXVECTOR3(+1, -1, 0), D3DXCOLOR(1, 1, 1, 1)));
	}

	const D3DXVECTOR2 tileSize(worldSize.x / m_worldGridSize.x, worldSize.y / m_worldGridSize.y);

	//Draw background
	{
		g_processManager->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
		g_processManager->GetDevice()->SetFVF(TempVertex::FVF);
		
		D3DXMATRIX s, t;
		D3DXMatrixTranslation(&t, drawCenter.x, drawCenter.y, 0);
		
		for (auto& iter : temp)
			iter.color = D3DXCOLOR(0, 0, 0, 1);
		D3DXMatrixScaling(&s, worldSize.x * (0.5f * (1 + BackgroundOutlineRatio)), worldSize.y * (0.5f * (1 + BackgroundOutlineRatio)), 1);
		g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
		g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &temp[0], sizeof(TempVertex));
		
		for (auto& iter : temp)
			iter.color = BackgroundColor;
		D3DXMatrixScaling(&s, worldSize.x * 0.5f, worldSize.y * 0.5f, 1);
		g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
		g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &temp[0], sizeof(TempVertex));
	}

	//Draw tile block
	for (int y = 0; y < m_worldGridSize.y; y++)
	{
		for (int x = 0; x < m_worldGridSize.x; x++)
		{
			auto& thisTile(m_gameWorld[y][x]);
			if (thisTile.first)
			{
				int tempCount(0);
				if (thisTile.first > 0)
					for (__int64 temp = thisTile.first; temp != 1; temp /= 2)
						tempCount++;

				//Set render position
				D3DXVECTOR2 renderPosition(x + 0.5f, y + 0.5f);
				if (m_timeAfterDeparture)
					renderPosition += D3DXVECTOR2(thisTile.second.x - x, thisTile.second.y - y) * (1 - pow(m_timeAfterDeparture / ArrivalTime, 2)); // (nextPosition - nowPosition) * movedTime
				//(TileKoordinatensystem -> WorldKoordinatensystem) + WorldCenterPosition
				(renderPosition.x *= tileSize.x) += drawCenter.x - (worldSize.x * 0.5f);
				(renderPosition.y *= tileSize.y) += drawCenter.y - (worldSize.y * 0.5f);

				//Draw Plane
				{
					g_processManager->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
					g_processManager->GetDevice()->SetFVF(TempVertex::FVF);

					D3DXMATRIX s, t;
					D3DXMatrixTranslation(&t, renderPosition.x, renderPosition.y, 0);

					for (auto& iter : temp)
						iter.color = D3DXCOLOR(0, 0, 0, 1);
					D3DXMatrixScaling(&s, tileSize.x * 0.5f, tileSize.y * 0.5f, 1);
					g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
					g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &temp[0], sizeof(TempVertex));

					if (thisTile.first > 0)
					{
						for (auto& iter : temp)
							iter.color = D3DXCOLOR(1, 1 - 0.1 * tempCount, 1 - 0.1 * tempCount, 1);
					}
					else
					{
						for (auto& iter : temp)
							iter.color = D3DXCOLOR(0.5, 0.5, 0.5, 1);
					}
					D3DXMatrixScaling(&s, tileSize.x * (0.5f * (1 - BlockOutlineRatio)), tileSize.y * (0.5f * (1 - BlockOutlineRatio)), 1);
					g_processManager->GetDevice()->SetTransform(D3DTS_WORLD, &(s * t));
					g_processManager->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &temp[0], sizeof(TempVertex));
				}

				//Draw Point
				if (thisTile.first > 0)
				{
					WCHAR tempBuffer[1000] = { NULL };
					RECT_ex tempRect(renderPosition.x, renderPosition.y, renderPosition.x, renderPosition.y);
					//Outline
					{
						RECT_ex outlineRect;
						m_font->DrawTextW(NULL, _i64tow(m_gameWorld[y][x].first, tempBuffer, 10), -1, &(tempRect - 1), DT_NOCLIP | DT_CENTER | DT_VCENTER, D3DXCOLOR(0, 0, 0, 1));
						m_font->DrawTextW(NULL, _i64tow(m_gameWorld[y][x].first, tempBuffer, 10), -1, &(tempRect + 1), DT_NOCLIP | DT_CENTER | DT_VCENTER, D3DXCOLOR(0, 0, 0, 1));
						outlineRect = tempRect;	tempRect.left--;	tempRect.top++;	tempRect.right--;	tempRect.bottom++;
						m_font->DrawTextW(NULL, _i64tow(m_gameWorld[y][x].first, tempBuffer, 10), -1, &outlineRect, DT_NOCLIP | DT_CENTER | DT_VCENTER, D3DXCOLOR(0, 0, 0, 1));
						outlineRect = tempRect;	tempRect.left++;	tempRect.top--;	tempRect.right++;	tempRect.bottom--;
						m_font->DrawTextW(NULL, _i64tow(m_gameWorld[y][x].first, tempBuffer, 10), -1, &outlineRect, DT_NOCLIP | DT_CENTER | DT_VCENTER, D3DXCOLOR(0, 0, 0, 1));
					}
					m_font->DrawTextW(NULL, _i64tow(m_gameWorld[y][x].first, tempBuffer, 10), -1, &tempRect, DT_NOCLIP | DT_CENTER | DT_VCENTER, D3DXCOLOR(1, 1, 1, 1));
				}
			}
		}
	}
}


void GameWorld::MoveBox(const MoveDirection & moveDirection)
{
	if (m_timeAfterDeparture)
		return;
	
	bool blockMoved(false);
	switch (moveDirection)
	{
	case MoveDirection::Left:
		for (int y = 0; y < m_worldGridSize.y; y++)
		{
			for (int x = 0; x < m_worldGridSize.x; x++)
			{
				if (m_gameWorld[y][x].first)
				{
					for (int nextX = 0; nextX < x + 1; nextX++)
					{
						if (m_nextWorld[y][nextX].second)
							continue;
						
						if (m_nextWorld[y][nextX].first)
						{
							m_nextWorld[y][nextX].second = true;
							if (m_nextWorld[y][nextX].first == m_gameWorld[y][x].first)
								m_scoreTempStack += (m_nextWorld[y][nextX].first *= 2);
							else
								continue;
						}
						else
							m_nextWorld[y][nextX].first = m_gameWorld[y][x].first;
						
						if (x != nextX)
							blockMoved = true;
						m_gameWorld[y][x].second = POINT_ex(nextX, y);
						break;
					}
				}
			}
		}
		break;
	case MoveDirection::Top:
		for (int x = 0; x < m_worldGridSize.x; x++)
		{
			for (int y = 0; y < m_worldGridSize.y; y++)
			{
				if (m_gameWorld[y][x].first)
				{
					for (int nextY = 0; nextY < y + 1; nextY++)
					{
						if (m_nextWorld[nextY][x].second)
							continue;

						if (m_nextWorld[nextY][x].first)
						{
							m_nextWorld[nextY][x].second = true;
							if (m_nextWorld[nextY][x].first == m_gameWorld[y][x].first)
								m_scoreTempStack += (m_nextWorld[nextY][x].first *= 2);
							else
								continue;
						}
						else
							m_nextWorld[nextY][x].first = m_gameWorld[y][x].first;
						
						if (y != nextY)
							blockMoved = true;
						m_gameWorld[y][x].second = POINT_ex(x, nextY);
						break;
					}
				}
			}
		}
		break;
	case MoveDirection::Right:
		for (int y = 0; y < m_worldGridSize.y; y++)
		{
			for (int x = m_worldGridSize.x - 1; x > 0 - 1; x--)
			{
				if (m_gameWorld[y][x].first)
				{
					for (int nextX = m_worldGridSize.x - 1; nextX > x - 1; nextX--)
					{
						if (m_nextWorld[y][nextX].second)
							continue;

						if (m_nextWorld[y][nextX].first)
						{
							m_nextWorld[y][nextX].second = true;
							if (m_nextWorld[y][nextX].first == m_gameWorld[y][x].first)
								m_scoreTempStack += (m_nextWorld[y][nextX].first *= 2);
							else
								continue;
						}
						else
							m_nextWorld[y][nextX].first = m_gameWorld[y][x].first;
						
						if (x != nextX)
							blockMoved = true;
						m_gameWorld[y][x].second = POINT_ex(nextX, y);
						break;
					}
				}
			}
		}
		break;
	case MoveDirection::Bottom:
		for (int x = 0; x < m_worldGridSize.x; x++)
		{
			for (int y = m_worldGridSize.y - 1; y > 0 - 1; y--)
			{
				if (m_gameWorld[y][x].first)
				{
					for (int nextY = m_worldGridSize.y - 1; nextY > y - 1; nextY--)
					{
						if (m_nextWorld[nextY][x].second)
							continue;

						if (m_nextWorld[nextY][x].first)
						{
							m_nextWorld[nextY][x].second = true;
							if (m_nextWorld[nextY][x].first == m_gameWorld[y][x].first)
								m_scoreTempStack += (m_nextWorld[nextY][x].first *= 2);
							else
								continue;
						}
						else
							m_nextWorld[nextY][x].first = m_gameWorld[y][x].first;

						if (y != nextY)
							blockMoved = true;
						m_gameWorld[y][x].second = POINT_ex(x, nextY);
						break;
					}
				}
			}
		}
		break;
	}


	for (int y = 0; y < m_worldGridSize.y; y++)
		for (int x = 0; x < m_worldGridSize.x; x++)
			m_nextWorld[y][x].second = false;


	if (blockMoved)
	{
		0 < ArrivalTime ?
			m_timeAfterDeparture = ArrivalTime :
			m_worldUpdated = true;
	}
	else
	{
		for (int y = 0; y < m_worldGridSize.y; y++)
			for (int x = 0; x < m_worldGridSize.x; x++)
				m_nextWorld[y][x].first = 0;
	}

}

#include <time.h>
bool GameWorld::CreateBlock(const __int64 & point)
{
	for (int y = 0; y < m_worldGridSize.y; y++)
		for (int x = 0; x < m_worldGridSize.x; x++)
			if (!m_gameWorld[y][x].first)
				goto FunctionExecutionIsCan;
	return false;
	FunctionExecutionIsCan:


	srand((unsigned int)time(NULL));
	int worldSize = m_worldGridSize.x * m_worldGridSize.y;

	bool* isBlocked = new bool[worldSize];
	for (int count = worldSize; count >= 0;)
		isBlocked[--count] = false;

	int count = 0;

	while (count != worldSize)
	{
		int realityPosition = -1;
		for (int randomPosition = rand() % (worldSize - count); randomPosition >= 0; randomPosition--)
		{
			if (isBlocked[realityPosition + 1])
				randomPosition++;
			realityPosition++;
		}

		if (!isBlocked[realityPosition])
		{
			int lastPositionY = realityPosition / m_worldGridSize.x;
			int lastPositionX = realityPosition - (lastPositionY * m_worldGridSize.x);

			if (m_gameWorld[lastPositionY][lastPositionX].first)
				isBlocked[realityPosition] = true;
			else
				return CreateBlock(POINT_ex(lastPositionX, lastPositionY), point);
		}
		count++;
	}
	return false;
}

bool GameWorld::CreateBlock(const POINT_ex & position, const __int64 & point)
{
	if (m_gameWorld[position.y][position.x].first)
		return false;

	m_gameWorld[position.y][position.x].first = point;
	return true;
}

