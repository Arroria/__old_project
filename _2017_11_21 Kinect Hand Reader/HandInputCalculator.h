#pragma once

#include "KinectManager.h"
enum class HandInputStatus
{
	UserChanged,
};

enum class SlidedDirection
{
	None,
	Up,
	Down,
	Left,
	Right
};

class HandInputCalculator
{
private:
	static __int64 m_queryPerformanceFrequency;

private:
	//Pair = KinectHandData + recordedTime
	std::list<std::pair<KinectHandData, __int64>> m_recordedHandDataList;
	
public:
	void SendHandData(const KinectHandData& handData);
	void CalculateHandMoving();

//OUTPUT
private:
	SlidedDirection m_slidedDirection;

public:
	bool IsSlidedAtUp()		{ return m_slidedDirection == SlidedDirection::Up; }
	bool IsSlidedAtDown()	{ return m_slidedDirection == SlidedDirection::Down; }
	bool IsSlidedAtLeft()	{ return m_slidedDirection == SlidedDirection::Left; }
	bool IsSlidedAtRight()	{ return m_slidedDirection == SlidedDirection::Right; }

public:
	HandInputCalculator();
	~HandInputCalculator();
};

