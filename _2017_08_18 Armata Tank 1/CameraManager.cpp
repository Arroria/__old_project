#include "MainHeader.h"
#include "CameraManager.h"

namespace AF
{
	CameraManager::CameraManager()
		: m_cameraEye(0, 0, -1)
		, m_cameraAt(0, 0, 0)
		, m_cameraUp(0, 1, 0)

		, m_viewAngle(NULL)
		, m_sightAngle(NULL)
	{
	}
	CameraManager::~CameraManager()
	{
	}



	void CameraManager::SetViewTransform(const D3DXVECTOR3 & eye, const D3DXVECTOR3 & at, const D3DXVECTOR3 & up)
	{
		D3DXMATRIX viewTransformMatrix;
		D3DXMatrixLookAtLH(&viewTransformMatrix, &eye, &at, &up);
		AF::g_processManager->GetDevice()->SetTransform(D3DTS_VIEW, &viewTransformMatrix);

		m_cameraEye	= eye;
		m_cameraAt	= at;
		m_cameraUp	= up;
	}

	void CameraManager::SetProjectionTransform(const float & viewingAngle, const float & sightRange)
	{
		RECT rc;	
		GetClientRect(AF::g_processManager->GetWindowInfo().hWnd, &rc);

		D3DXMATRIX projectionTransformMatrix;
		D3DXMatrixPerspectiveFovLH(&projectionTransformMatrix, D3DXToRadian(viewingAngle), 1, 1, sightRange);
		AF::g_processManager->GetDevice()->SetTransform(D3DTS_PROJECTION, &projectionTransformMatrix);

		m_viewAngle		= viewingAngle;
		m_sightAngle	= sightRange;
	}
}