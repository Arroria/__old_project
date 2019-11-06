#include "DXUT.h"
#include "AIPlayer.h"

#include "Map.h"


AIPlayer::AIPlayer()
	: m_isEven(rand() & 1)
{
	m_attackData.fill(false);
}
AIPlayer::~AIPlayer()
{
}



bool AIPlayer::Attack(POINT & out)
{
	if (m_scanData.size())
	{
		POINT scanData = m_scanData.front();
		m_scanData.pop_front();
		
		if (m_attackData[Map::PointToLine(scanData)])
			return false;
		out = scanData;
		return true;
	}
	else if (m_hitData.size())			return NextToHit(out);
	else if (FindGrid(out, m_isEven))	return true;
	else								return FindGrid(out, !m_isEven);

	return false;
}



bool AIPlayer::NextToHit(POINT & out)
{
	auto& hitData = m_hitData.front();

	UINT nonAttackCount = 0;
	for (UINT i = 0; i < 4; i++)
	{
		POINT sample = hitData;
		switch (i)
		{
		case 0: sample.x++;	break;
		case 1: sample.y++;	break;
		case 2: sample.x--;	break;
		case 3: sample.y--;	break;
		}

		if (sample.x >= 0 &&
			sample.y >= 0 &&
			sample.x < 10 &&
			sample.y < 10 &&
			!m_attackData[Map::PointToLine(sample)])
			nonAttackCount++;
	}

	if (!nonAttackCount)
	{
		m_hitData.pop_front();
		return false;
	}

	UINT randomIndex = rand() % nonAttackCount;
	UINT index = 0;
	do
	{
		POINT sample = hitData;
		switch (index)
		{
		case 0: sample.x++;	break;
		case 1: sample.y++;	break;
		case 2: sample.x--;	break;
		case 3: sample.y--;	break;
		}

		if (sample.x < 0 ||
			sample.y < 0 ||
			sample.x > 9 ||
			sample.y > 9 ||
			m_attackData[Map::PointToLine(sample)])
			index++;
		else if (randomIndex)
		{
			randomIndex--;
			index++;
		}
		else
			break;
	} while (true);

	out = hitData;
	switch (index)
	{
	case 0: out.x++;	break;
	case 1: out.y++;	break;
	case 2: out.x--;	break;
	case 3: out.y--;	break;
	}
	return true;
}

bool AIPlayer::FindGrid(POINT & out, const bool& isEven)
{
	auto ForEven = [&isEven](const UINT& pos)->UINT
	{
		return pos + (((pos / 10) & 1) ^ (int)isEven);
	};

	UINT canAttackPlace = 0;
	for (UINT i = 0; i < 100; i += 2)
	{
		if (!m_attackData[ForEven(i)])
			canAttackPlace++;
	}

	if (!canAttackPlace)
		return false;

	UINT randomIndex = rand() % canAttackPlace;
	UINT index = 0;
	do
	{
		if (m_attackData[ForEven(index)])
			index += 2;
		else if (randomIndex)
		{
			randomIndex--;
			index += 2;
		}
		else
			break;
	} while (true);

	out = Map::LineToPoint(ForEven(index));
	return true;
}

void AIPlayer::PutAttackData(const POINT & data)	{ m_attackData[Map::PointToLine(data)] = true; }
void AIPlayer::PutHitData	(const POINT & data)	{ m_hitData.push_back(data); }
void AIPlayer::PutScanData	(const POINT & data)	{ m_scanData.push_back(data); }
