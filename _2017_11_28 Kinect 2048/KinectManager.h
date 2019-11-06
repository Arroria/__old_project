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
	/// <summary>
	/// 키넥트 센서를 인식하고 필요한 데이터를 불러오거나 생성합니다.
	/// </summary>
	/// <returns>함수의 성공여부를 HRESULT로 반환합니다.</returns>
	HRESULT Initialize();

	/// <summary>
	/// 키넥트 센서로부터 인식된 몸체들의 손의 상태를 얻고, 연산 후 저장합니다.
	/// </summary>
	/// <returns>함수의 성공여부를 HRESULT로 반환합니다.</returns>
	HRESULT CalculateHandState();


//OUTPUT
private:
	std::list<int> m_bodyPriority;
	KinectHandData	m_handData[BODY_COUNT][(UINT)Hand::Count];

public:
	/// <summary>
	/// 키넥트 센서에게서 부터 얻어온 몸체들 중에서 가장 먼저 인식된 몸체의 번호를 가져옵니다.
	/// </summary>
	/// <returns>0 ~ 5의 값을 반환하며, 인식된 몸체가 없을경우 -1을 반환합니다.</returns>
	int	GetMainBody() { return m_bodyPriority.empty() ? -1 : m_bodyPriority.front(); }
	/// <summary>
	/// 저장된 몸체의 손 데이터를 얻어옵니다.
	/// </summary>
	/// <param name="bodyIndex">손 데이터를 얻어올 몸체의 번호입니다.</param>
	/// <param name="hand">손 데이터를 얻어올 손이 왼손인지, 오른손인지 전달합니다.</param>
	/// <returns>정상적인 인자를 전달할경우 그에 맞는 손 데이터를, 비정상적인 인자를 전달할경우 비어있는 구조체를 반환합니다.</returns>
	KinectHandData GetHandData(const UINT& bodyIndex, const Hand& hand);

public:
	KinectManager();
	~KinectManager();
};

