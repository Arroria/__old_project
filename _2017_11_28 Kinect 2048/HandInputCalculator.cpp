#include "stdafx.h"
#include "HandInputCalculator.h"


#define CheckTime 0.5

#define CheckRange 0.2

//HandInputCalculator::c_queryPerformanceFrequency = NULL;

__int64 HandInputCalculator::m_queryPerformanceFrequency = NULL;
HandInputCalculator::HandInputCalculator() :
	m_slidedDirection(SlidedDirection::None),
	m_isClicked(false)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_queryPerformanceFrequency);
}
HandInputCalculator::~HandInputCalculator()
{
}


void HandInputCalculator::SendHandData(const KinectHandData & handData)
{
	__int64 time = NULL;
	QueryPerformanceCounter((LARGE_INTEGER*)(&time));
	m_recordedHandDataList.push_back(std::make_pair(handData, time));

	while (true)
	{
		double prevTime = (double)m_recordedHandDataList.front().second / m_queryPerformanceFrequency;
		double nowTime = (double)m_recordedHandDataList.back().second / m_queryPerformanceFrequency;
		if (nowTime - prevTime > CheckTime)
			m_recordedHandDataList.erase(m_recordedHandDataList.begin());
		else
			break;
	}
}

void HandInputCalculator::CalculateHandMoving()
{
	if (m_recordedHandDataList.size() < 2)
	{
		m_slidedDirection = SlidedDirection::None;
		return;
	}

	auto& prevData = m_recordedHandDataList.front();
	auto& nowData = m_recordedHandDataList.back();

	//if ((double)(nowData.second - prevData.second) / HandInputCalculator::m_queryPerformanceFrequency < CheckTime)
	//	m_slidedDirection = SlidedDirection::None;
	//else
	{
		float xRange = nowData.first.x - prevData.first.x;
		float yRange = nowData.first.y - prevData.first.y;

		auto IsSameHandType = [this](const HandState& handType)->bool
		{
			UINT count = 0;
			UINT sameCount = 0;
			for (auto& iter : m_recordedHandDataList)
			{
				if (iter.first.handState == handType)
					sameCount++;
				count++;
			}
			return sameCount / (float)count > 0.8f;
		};

		if (IsSameHandType(HandState::HandState_Open))
		{
			bool listClear = true;

				 if (xRange < -CheckRange)	m_slidedDirection = SlidedDirection::Left;
			else if (xRange > CheckRange)	m_slidedDirection = SlidedDirection::Right;
			else if (yRange < -CheckRange)	m_slidedDirection = SlidedDirection::Up;
			else if (yRange > CheckRange)	m_slidedDirection = SlidedDirection::Down;
			else
			{
				m_slidedDirection = SlidedDirection::None;
				listClear = false;
			}
				
			if (listClear)
				 m_recordedHandDataList.clear();
		}
		else
			m_slidedDirection = SlidedDirection::None;
	}
}

void HandInputCalculator::CalculateHandClick()
{
	enum class ClickState
	{
		None,
		ClickReady,
		Clicking,
		Clicked,
	}
	clickState(ClickState::None);

	__int64 handClickStartTime = NULL;
	for (auto& iter : m_recordedHandDataList)
	{
		switch (clickState)
		{
		case ClickState::None:
			if (iter.first.handState == HandState::HandState_Open)
				clickState = ClickState::ClickReady;
			break;
		case ClickState::ClickReady:
			if (iter.first.handState == HandState::HandState_Closed)
			{
				clickState = ClickState::Clicking;
				handClickStartTime = iter.second;
			}
			break;
		case ClickState::Clicking:
			if (iter.first.handState == HandState::HandState_Open)
			{
				double clickTimeInterval = (double)(iter.second - handClickStartTime) / m_queryPerformanceFrequency;
				if (0.1 < clickTimeInterval && clickTimeInterval < 0.2)
					clickState = ClickState::Clicked;
			}
			break;
		default:
			break;
		}
	}
	
	if (clickState == ClickState::Clicked)
	{
		m_isClicked = true;
		m_recordedHandDataList.clear();
	}
	else
		m_isClicked = false;
}
