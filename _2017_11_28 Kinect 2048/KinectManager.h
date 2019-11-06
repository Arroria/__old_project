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
	/// Ű��Ʈ ������ �ν��ϰ� �ʿ��� �����͸� �ҷ����ų� �����մϴ�.
	/// </summary>
	/// <returns>�Լ��� �������θ� HRESULT�� ��ȯ�մϴ�.</returns>
	HRESULT Initialize();

	/// <summary>
	/// Ű��Ʈ �����κ��� �νĵ� ��ü���� ���� ���¸� ���, ���� �� �����մϴ�.
	/// </summary>
	/// <returns>�Լ��� �������θ� HRESULT�� ��ȯ�մϴ�.</returns>
	HRESULT CalculateHandState();


//OUTPUT
private:
	std::list<int> m_bodyPriority;
	KinectHandData	m_handData[BODY_COUNT][(UINT)Hand::Count];

public:
	/// <summary>
	/// Ű��Ʈ �������Լ� ���� ���� ��ü�� �߿��� ���� ���� �νĵ� ��ü�� ��ȣ�� �����ɴϴ�.
	/// </summary>
	/// <returns>0 ~ 5�� ���� ��ȯ�ϸ�, �νĵ� ��ü�� ������� -1�� ��ȯ�մϴ�.</returns>
	int	GetMainBody() { return m_bodyPriority.empty() ? -1 : m_bodyPriority.front(); }
	/// <summary>
	/// ����� ��ü�� �� �����͸� ���ɴϴ�.
	/// </summary>
	/// <param name="bodyIndex">�� �����͸� ���� ��ü�� ��ȣ�Դϴ�.</param>
	/// <param name="hand">�� �����͸� ���� ���� �޼�����, ���������� �����մϴ�.</param>
	/// <returns>�������� ���ڸ� �����Ұ�� �׿� �´� �� �����͸�, ���������� ���ڸ� �����Ұ�� ����ִ� ����ü�� ��ȯ�մϴ�.</returns>
	KinectHandData GetHandData(const UINT& bodyIndex, const Hand& hand);

public:
	KinectManager();
	~KinectManager();
};

