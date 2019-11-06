#pragma once
#include <queue>

struct KinectHandData
{
	HandState handState;
	float x;
	float y;

	KinectHandData() : handState(HandState::HandState_NotTracked), x(NULL), y(NULL) {}
};
enum class Hand
{
	Left = 0,
	Right = 1,

	Count = (Right + 1)
};

class KinectManager
{
private:
	IKinectSensor* m_kinectSensor;

	IBodyFrameReader* m_bodyFrameReader;
	ICoordinateMapper* m_coordinateMapper;


//Main Kinect Secsor
public:
	HRESULT Initialize();

	HRESULT CalculateHandState();


//OUTPUT
private:
	std::list<int> m_bodyPriority;
	KinectHandData	m_handData[BODY_COUNT][(UINT)Hand::Count];

public:
	int	GetMainBody() { return m_bodyPriority.empty() ? -1 : m_bodyPriority.front(); }
	KinectHandData GetHandData(const UINT& bodyIndex, const Hand& hand);

public:
	KinectManager();
	~KinectManager();
};

