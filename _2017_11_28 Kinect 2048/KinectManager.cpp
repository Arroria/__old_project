#include "stdafx.h"
#include "KinectManager.h"



KinectManager::KinectManager() :
	m_kinectSensor(nullptr),
	
	m_bodyFrameReader(nullptr),
	m_coordinateMapper(nullptr)
{
}
KinectManager::~KinectManager()
{
	SAFE_RELEASE( m_bodyFrameReader );
	SAFE_RELEASE( m_coordinateMapper );

	if (m_kinectSensor)
	{
		m_kinectSensor->Close();
		m_kinectSensor->Release();
	}
}


HRESULT KinectManager::Initialize()
{
	HRESULT result(E_FAIL);
	if (SUCCEEDED(GetDefaultKinectSensor(&m_kinectSensor)))
	{
		IBodyFrameSource* bodyFrameSource = nullptr;
		if (SUCCEEDED(m_kinectSensor->Open()) &&
			SUCCEEDED(m_kinectSensor->get_CoordinateMapper(&m_coordinateMapper)) &&
			SUCCEEDED(m_kinectSensor->get_BodyFrameSource(&bodyFrameSource)) &&
			SUCCEEDED(bodyFrameSource->OpenReader(&m_bodyFrameReader)))
			result = S_OK;

		if (bodyFrameSource)
			bodyFrameSource->Release();
	}
	return result;
}

HRESULT KinectManager::CalculateHandState()
{
	if (!m_bodyFrameReader)
		return E_FAIL;

	IBodyFrame* bodyFrame = nullptr;
	if (SUCCEEDED( m_bodyFrameReader->AcquireLatestFrame(&bodyFrame) ))
	{
		IBody* bodyList[BODY_COUNT] = { 0 };
		if (SUCCEEDED( bodyFrame->GetAndRefreshBodyData(_countof(bodyList), bodyList) ))
		{
			for (int bodyIndex = 0; bodyIndex < BODY_COUNT; ++bodyIndex)
			{
				IBody* body = bodyList[bodyIndex];
				BOOLEAN isBodyTracked = false;

				if (body && SUCCEEDED( body->get_IsTracked(&isBodyTracked) ))
				{
					if (isBodyTracked)
					{
						for (auto iter = m_bodyPriority.begin(); iter != m_bodyPriority.end();iter++)
							if (*iter == bodyIndex)
								goto temp;
						m_bodyPriority.push_back(bodyIndex);
						temp:
						//auto iter = m_bodyPriority.begin();
						//while (*iter != bodyIndex)
						//{
						//	if (iter++ == m_bodyPriority.end())
						//	{
						//		m_bodyPriority.push_back(bodyIndex);
						//		break;
						//	}
						//}

						Joint bodyJointList[JointType_Count];
						if (SUCCEEDED(body->get_HandLeftState(&(m_handData[bodyIndex][(UINT)Hand::Left].handState))) &&
							SUCCEEDED(body->get_HandRightState(&(m_handData[bodyIndex][(UINT)Hand::Right].handState))) &&
							SUCCEEDED(body->GetJoints(_countof(bodyJointList), bodyJointList)))
						{
							DepthSpacePoint depthPoint = { 0 };
							m_coordinateMapper->MapCameraPointToDepthSpace(bodyJointList[JointType::JointType_HandLeft].Position, &depthPoint);
							m_handData[bodyIndex][(UINT)Hand::Left].x = depthPoint.X / 512;
							m_handData[bodyIndex][(UINT)Hand::Left].y = depthPoint.Y / 424;
							m_coordinateMapper->MapCameraPointToDepthSpace(bodyJointList[JointType::JointType_HandRight].Position, &depthPoint);
							m_handData[bodyIndex][(UINT)Hand::Right].x = depthPoint.X / 512;
							m_handData[bodyIndex][(UINT)Hand::Right].y = depthPoint.Y / 424;
						}
					}
					else
					{
						for (auto iter = m_bodyPriority.begin(); iter != m_bodyPriority.end();)
						{
							if (*iter == bodyIndex)
							{
								iter = m_bodyPriority.erase(iter);
								m_bodyPriority.push_back(bodyIndex);
							}
							else
								iter++;

							m_handData[bodyIndex][(UINT)Hand::Left].handState = m_handData[bodyIndex][(UINT)Hand::Right].handState = (HandState)(-1);
							m_handData[bodyIndex][(UINT)Hand::Left].x =
							m_handData[bodyIndex][(UINT)Hand::Left].y = 
							m_handData[bodyIndex][(UINT)Hand::Right].x =
							m_handData[bodyIndex][(UINT)Hand::Right].y = NULL;
						}
					}
				}
			}
		}

		for (int i = 0; i < _countof(bodyList); ++i)
			SAFE_RELEASE( bodyList[i] );
	}
	SAFE_RELEASE( bodyFrame );
	return S_OK;
}


KinectHandData KinectManager::GetHandData(const UINT & bodyIndex, const Hand & hand)
{
	return (bodyIndex != -1 && bodyIndex < BODY_COUNT && (UINT)hand < (UINT)Hand::Count) ?
		m_handData[bodyIndex][(UINT)hand] :
		KinectHandData();
}
