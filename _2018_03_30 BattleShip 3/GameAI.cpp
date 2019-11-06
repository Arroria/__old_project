#include "DXUT.h"
#include "GameAI.h"

#include "Map.h"


GameAI::GameAI()
	: m_isEven(rand() & 1)
	, m_attackCount(0)
{
	m_attackData.fill(false);
	m_item.fill(true);
}
GameAI::~GameAI()
{
}



bool GameAI::Attack(POINT & output)
{
	if (m_scanData.size())
	{
		POINT pos = m_scanData.front();
		m_scanData.erase(m_scanData.begin());
		auto& tile = m_attackData[Map::PointToLine(pos)];
		if (!tile)
		{
			tile = true;
			output = pos;
			return true;
		}
		return false;
	}

	if (m_hitData.size())
		return NextToHitFine(output);

	if (!GridFind(output, m_isEven))
		return GridFind(output, !m_isEven);
	return true;
}

bool GameAI::DeclarationUseItem(UINT & itemType)
{
	itemType = 3;
	return true;

	UINT leftItem = 0;
	for (auto& itemAble : m_item)
	{
		if (itemAble)
			leftItem++;
	}
	if (!leftItem)
		return false;

	UINT num = rand() % leftItem;
	UINT result = 0;
	do
	{
		if (!m_item[result])
			result++;
		else if (num)
		{
			num--;
			result++;
		}
		else
			break;
	} while (true);

	itemType = result;
	m_item[itemType] = false;
	return true;
}



void GameAI::PutHitData(const POINT & hitPos)
{
	m_hitData.push_back(hitPos);
}

void GameAI::PutScanData(const POINT & scanPos)
{
	m_scanData.push_back(scanPos);
}

void GameAI::PutAttackData(const POINT & attackPos)
{
	m_attackData[attackPos.x + attackPos.y * 10] = true;
}



bool GameAI::NextToHitFine(POINT & output)
{
	auto& dataIter = m_hitData.begin();
	if (dataIter == m_hitData.end())
		return false;

	POINT hitPos = (*dataIter);

	bool dirAble[4];
	UINT ableCount = 0;
	
	for (UINT i = 0; i < 4; i++)
	{
		POINT checkPos = hitPos;
		switch (i)
		{
		case 0:	checkPos.x += 1;	break;
		case 1:	checkPos.y += 1;	break;
		case 2:	checkPos.x -= 1;	break;
		case 3:	checkPos.y -= 1;	break;
		}
		if (checkPos.x < 0 ||
			checkPos.y < 0 ||
			checkPos.x > 9 ||
			checkPos.y > 9 ||
			m_attackData[checkPos.x + checkPos.y * 10])
			dirAble[i] = false;
		else
		{
			dirAble[i] = true;
			ableCount++;
		}
	}

	if (!ableCount)
	{
		m_hitData.erase(dataIter);
		return false;
	}

	UINT num = rand() % ableCount;
	UINT result = 0;
	do
	{
		if (!dirAble[result])
			result++;
		else if (num)
		{
			num--;
			result++;
		}
		else
			break;
	} while (true);


	output = hitPos;
	switch (result)
	{
	case 0:	output.x += 1;	break;
	case 1:	output.y += 1;	break;
	case 2:	output.x -= 1;	break;
	case 3:	output.y -= 1;	break;
	}

	return true;
}

bool GameAI::GridFind(POINT & output, const bool & isEven)
{
	auto ToGrid = [&isEven](const UINT& pos)->UINT
	{
		bool lineBinary = (pos / 10) & 1;
		return pos + (lineBinary ^ isEven ? 1 : 0) ;
	};

	UINT peaceCount = 0;
	for (UINT i = 0; i < 100; i += 2)
	{
		if (!m_attackData[ToGrid(i)])
			peaceCount++;
	}
	if (!peaceCount)
		return false;

	UINT num = rand() % peaceCount;
	UINT result = 0;
	do
	{
		if (m_attackData[ToGrid(result)])
			result += 2;
		else if (num)
		{
			num--;
			result += 2;
		}
		else
			break;
	} while (true);

	result = ToGrid(result);
	output.x = result % 10;
	output.y = result / 10;
	return true;
}



bool GameAI::LineAttack(UINT & line, bool & isLookAtY)
{
	POINT randomPos;
	if (!_33Attack(randomPos))
		return false;

	isLookAtY = rand() & 1;
	line = (isLookAtY ? randomPos.x : randomPos.y);
	return true;
}

bool GameAI::_33Attack(POINT & output)
{
	while (true)
	{
		UINT pos = rand() % 100;
		if (m_attackData[pos])
			continue;

		output.x = pos % 10;
		output.y = pos / 10;
		return true;
	}
}

bool GameAI::Scanning(POINT & output)
{
	return _33Attack(output);
}
