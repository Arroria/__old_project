#include "DXUT.h"
#include "GameAI.h"


GameAI::GameAI()
	: m_isEven(rand() & 0b1)
{
	m_lastAttackPoint.x = m_lastAttackPoint.y = -1;
	m_myData.fill(false);
	m_dir.fill(false);
}
GameAI::~GameAI()
{
}



bool GameAI::Output(POINT& output)
{
	if (FindHit(output))					{ m_lastAttackPoint = output; return true; }
	if (FindDiagonal(output, m_isEven))		{ m_lastAttackPoint = output; return true; }
	if (FindDiagonal(output, !m_isEven))	{ m_lastAttackPoint = output; return true; }
	return false;
}

void GameAI::Input(const bool & attackSucceeded, const bool & hit)
{
	if (attackSucceeded)
	{
		MyData(m_lastAttackPoint.x, m_lastAttackPoint.y) = true;
		if (hit)
		{
			m_hitPos.push_back(m_lastAttackPoint);
		}
	}
}



bool GameAI::FindHit(POINT & output)
{
	if (m_hitPos.size())
	{
		POINT pos = m_hitPos.front(); 
		UINT dir = 0; {
			UINT leftDir = 0; {
				for (UINT i = 0; i < 4; i++)
				{
					if (!m_dir[i])
						leftDir++;
				}
			}
			UINT count = rand() % leftDir;
			do
			{
				if (m_dir[dir])
					dir++;
				else if (count)
				{
					count--;
					dir++;
				}
				else
					break;
			} while (true);
		}
		switch (dir)
		{
		case 0:	pos.x++;	break;
		case 1:	pos.y++;	break;
		case 2:	pos.x--;	break;
		case 3:	pos.y--;	break;
		}

		m_dir[dir] = true;
		bool attack = false;
		if (pos.x >= 0 &&
			pos.y >= 0 &&
			pos.x < 10 &&
			pos.y < 10 &&
			!MyData(pos.x, pos.y))
		{
			output.x = pos.x;
			output.y = pos.y;
			attack = true;
		}

		//Erase
		bool isAllBlock = true;
		for (UINT i = 0; i < 4; i++)
		{
			if (!m_dir[i])
			{
				isAllBlock = false;
				break;
			}
		}
		if (isAllBlock)
		{
			m_hitPos.erase(m_hitPos.begin());
			m_dir.fill(false);
		}

		return attack;
	}
	return false;
}

bool GameAI::FindDiagonal(POINT & output, const bool& even)
{
	UINT pos = 0; {
		UINT leftPos = 0; {
			for (UINT i = 0; i < 50; i++)
			{
				UINT evenOffset = (i % 10 < 5) ^ even ? 1 : 0;
				if (!m_myData[i * 2 + evenOffset])
					leftPos++;
			}
		}
		if (!leftPos)
			return false;
	
		UINT count = rand() % leftPos;
		do
		{
			UINT evenPos = ((pos % 20 < 10) ^ even ? 1 : 0) + pos;
			if (m_myData[evenPos])
				pos += 2;
			else if (count)
			{
				count--;
				pos += 2;
			}
			else
				break;
		} while (true);
		pos = ((pos % 20 < 10) ^ even ? 1 : 0) + pos;
	}

	if (!m_myData[pos])
	{
		output.x = pos % 10;
		output.y = pos / 10;
		cout << pos << " " << output.x << " " << output.y << endl;
		return true;
	}
	return false;
}







/*

GameAI2::GameAI2()
	: 
	
	m_lastAttackPos{ -1, -1 }
{
	/* Ship *//* {
		m_enemyShip.resize(5);
		m_enemyShip[0] = std::make_pair(2, true);
		m_enemyShip[1] = std::make_pair(2, true);
		m_enemyShip[2] = std::make_pair(3, true);
		m_enemyShip[3] = std::make_pair(3, true);
		m_enemyShip[4] = std::make_pair(4, true);
	}

	m_map.fill(TileData());
}
GameAI2::~GameAI2()
{
}



void GameAI2::Calculate()
{
}

void GameAI2::ResultUpdate(const bool & attackHit, const UINT & attackKill)
{
	auto& lastAttackTile = m_map[PointToLine(m_lastAttackPos)];
	
	lastAttackTile.m_attacked = true;
	if (attackHit)
		lastAttackTile.m_isEnemy = true;


	if (attackKill)
	{
		auto HitCheck = [&, this](POINT pos)->bool
		{
			UINT linePos = PointToLine(pos);
			return !OutOfMap(pos) && !m_map[linePos].m_isExpired && m_map[linePos].m_isEnemy;
		};
		auto DirPluser = [](const POINT& pos, const UINT& dir)->POINT
		{
			POINT out = pos;
			switch (dir)
			{
			case 0:	out.x += 1;	break;
			case 1:	out.y += 1;	break;
			case 2:	out.x -= 1;	break;
			case 3:	out.y -= 1;	break;
			}
			return out;
		};

		if (attackKill == 2)
		{
			int dir = -1;
			UINT hitPoint = 0;
			for (UINT i = 0; i < 4; i++)
			{
				if (HitCheck(DirPluser(m_lastAttackPos, i)))
				{
					dir = i;
					hitPoint++;
				}
			}

			if (hitPoint == 1)
			{
				m_map[PointToLine(m_lastAttackPos)].m_isExpired = true;
				m_map[PointToLine(DirPluser(m_lastAttackPos, dir))].m_isExpired = true;
			}
		}

		bool xP, yP, xM, yM;
		xP = NextToHitCheck(m_lastAttackPos, +1, 0);
		xM = NextToHitCheck(m_lastAttackPos, -1, 0);
		yP = NextToHitCheck(m_lastAttackPos, 0, +1);
		yM = NextToHitCheck(m_lastAttackPos, 0, -1);

		UINT nextToHit = 0;
		if (xP) nextToHit++;
		if (xM) nextToHit++;
		if (yP) nextToHit++;
		if (yM) nextToHit++;

		if (!nextToHit) {}
		else (nextToHit == 1)
		{
			
		}
		else
		{
			//예외처리 필요함
		}
	}
}



UINT GameAI2::ShipCount(const UINT& size)
{
	UINT liveCount = 0;
	for (auto& ship : m_enemyShip)
	{
		if (ship.second && (!size || size == ship.first))
			liveCount++;
	}
	return liveCount;
}


*/